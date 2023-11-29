#include <stdio.h>
#include <string.h>

unsigned int global_seed = 1022; //seed เริ่มต้น
int goal = 21; //goal to win

//struct สำหรับใช้ในฟังก์ชั่น update เพื่อเก็บข้อมูลผู้เล่น(ชื่อ , คะแนนฝั่งเรา, คะแนนฝั่งตรงข้าม)
typedef struct{
    char name[100];
    int my_score;
    int com_score;
} player;

//bool type variable
typedef enum{
    true=1,
    false=0
} bool;

//ฟังก์ชั่นสุ่มตัวเลขตั้งแต่ 0 ถึง range-1  -  O(1) 
unsigned int random(unsigned int seed,int range){
    global_seed += (9*seed+173)%range;
    global_seed %= 400009;
    return (7*seed+233)%range;
}

//ฟังก์ชั่น shuffle เด็คโดยใช้ two pointer  -  O(n)
void shuffle(char* deck[],int size){
    while(size--){
        int i = random(global_seed,size+1);
        int j = size;
        char* temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

//ฟังก์ชั่นสำหรับปริ้นการ์ดจากเด็คใช้ในฟังก์ชั่น display  -  O(n)
void straight_print(char deck[][5],int index){
    if(index != 1){
        straight_print(deck,index-1);
    }
    printf("%s ",deck[index-1]);
}

//ฟังก์ชั่นสำหรับคำนวน score จาก deck  -  O(n)
int score(char deck[][5],int size){
    int sum=0;
    int ace=0;//ตัวแปรเก็บว่ามีไพ่ ACE กี่ใบ
    while(size--){
        char n = deck[size][0];
        if(n == 'J' || n=='K' || n=='Q' || n=='1'){
            sum+=10;
        }else if(n == 'A'){
            sum+=10;
            ace++;
        }else{
            sum+=n-'0';
        }
        if(sum > goal && ace>0){//ถ้าแต้มรวมมากกว่า 21 ไพ่ ACE จะเปลี่ยนจาก 10 -> 1 แต้ม
            sum-=9;
            ace--;
        }
    }
    return sum;
}

//ฟังก์ชั่นแสดงสถานการณ์ในเกม ณ ขณะนั้น  -  O(1)
void display(char deck1[][5],char deck2[][5],char* name1,char* name2,int size1,int size2){
    printf("   %s: ",name1);
    straight_print(deck1,size1);
    printf("(%d)",score(deck1,size1));
    printf("     %s: ",name2);
    straight_print(deck2,size2);
    printf("(%d)",score(deck2,size2)); 
    printf("\n");
}

//ฟังก์ชั่น นำการ์ดใบบนสุดออกจากสำรับ  -  O(1)
char* pop_card(char* deck[],int* size){
    *size -= 1;
    return deck[*size];
}

//ฟังก์ชั่น เพิ่มการ์ดไปยังบนสุดสำรับ  -  O(1)
void add_card(char deck[][5],int* size,char* card){
    strcpy(deck[*size],card);
    *size += 1;
}

//ฟังก์ชั่นสำหรับแสดงเด็คสำหรับเล่น  -  O(n)
void print_deck(char* deck[],int size){
    printf("\n   ");
    for(int i=0;i<size;i++){
        if(i%13==0 && i!=0){printf("\n\n   ");}
        printf(" %s ",deck[i]);
    }
    printf("\n\n");
}

//สร้าง seed จาก string โดยแปลง char -> int แล้วนำไปคูณกับ seed
void seed_generator(char str[]){
    int i = 0;
    while(1){
        if(str[i] == '\0'){break;}
        global_seed *= (int)str[i];
        global_seed = global_seed% 400009;//mod ไม่ให้ตัวเลขใหญ่เกิน
        i++;
    }
}

//ฟังก์ชั่น update ข้อมูลในไฟล์  -  O(n)
void update(char* nme,int scr,int c_scr,bool ismember){ 
    FILE *data_input = fopen("data.txt","r");
    int num_data; //จำนวนข้อมูล
    int member_index; //ตำแหน่งของผู้เล่นในไฟล์

    //อ่านค่าว่ามีข้อมูลเก็บอยู่กี่ข้อมูล
    fscanf(data_input,"%d",&num_data);
    //ถ้าไม่ใช่ member เพิ่มจำนวนข้อมูล +1
    if(!ismember)
        num_data++;
    player player_data[num_data];// สำหรับเก็บข้อมูลที่อ่านมา

    int index=0;
    //อ่านไฟล์ทีละบรรทัดและนำบันทึกลงในตัวแปร  -  O(n)
    while(fscanf(data_input,"%s %d %d",player_data[index].name,&player_data[index].my_score,&player_data[index].com_score) != EOF){
        //ถ้าเจอข้อมูลที่ชื่อตรงกับชื่อ member จะบันทึกตำแหน่งไว้
        if(strcmp(player_data[index].name,nme) == 0){
            member_index = index;
        }
        index++;
    }
    
    //เช็คว่าเป็นสมาชิกหรือเปล่า(มีข้อมูลในไฟล์) 
    if(!ismember){//   ถ้าไม่เป็นจะบันทึกข้อมูลไปยัง index สุดท้าย(เพิ่มใหม่)
        strcpy(player_data[index].name,nme);
        player_data[index].my_score = scr;
        player_data[index].com_score = c_scr;
    }else{//   ถ้าเป็นจะบันทึกข้อมูลไปยัง member_index
        player_data[member_index].my_score = scr;
        player_data[member_index].com_score = c_scr;
    }

    char output[2000]=""; // สำหรับเก็บข้อมูลที่จะเขียนลงไป
    sprintf(output,"%d\n",num_data); // เริ่มด้วยการบันทึก จำนวนข้อมูล ในบรรทัดแรก

    //จากนั้นบันทึกข้อมูลจากตัวแปรที่อ่านมาลงใน output ทีละตัว  -  O(n)
    for(int i=0;i<num_data;i++){
        char tmp[150];
        sprintf(tmp,"%s %d %d",player_data[i].name,player_data[i].my_score,player_data[i].com_score);
        strcat(output,tmp);//denote output += tmp;
        if(i != num_data-1){strcat(output,"\n");}//denote output += "\n";
    }

    fclose(data_input);
    FILE *data_output = fopen("data.txt","w");
    fprintf(data_output,output); //write output
    fclose(data_output);
}

int main(){
    char* deck[] = {"A-\3","2-\3","3-\3","4-\3","5-\3","6-\3","7-\3","8-\3","9-\3","10-\3","J-\3","Q-\3","K-\3",
                    "A-\4","2-\4","4-\4","4-\4","5-\4","6-\4","7-\4","8-\4","9-\4","10-\4","J-\4","Q-\4","K-\4",
                    "A-\5","2-\5","4-\5","4-\5","5-\5","6-\5","7-\5","8-\5","9-\5","10-\5","J-\5","Q-\5","K-\5",
                    "A-\6","2-\6","4-\6","4-\6","5-\6","6-\6","7-\6","8-\6","9-\6","10-\6","J-\6","Q-\6","K-\6"};
    int deck_size = 52;// จำนวนการ์ดในเด็คที่ใช้เล่น
    char name[200];
    int player_score = 0;
    int computer_score = 0;
    char player_deck[10][5];
    int player_size=0; // จำนวนการ์ดผู้เล่น
    char computer_deck[10][5];
    int computer_size=0; // จำนวนการ์ดคอมพิวเตอร์
    bool playing = true;

    printf("==========================================================================\n\n");
    printf("    Welcome to Blackjack! This is the card deck for playing this game\n");
    print_deck(deck,deck_size);
    printf("==========================================================================\n\n");

    printf("Please enter your name.\n-> ");
    scanf("%s",name);
    seed_generator(name);//สร้าง seed สำหรับการสุ่มโดยใช้ name
    
//======================= LOAD =================================
    char loaded_name[100];
    int loaded_pscore;
    int loaded_cscore;
    int num_data;
    bool member = false;//ตัวแปรเช็คว่าเป็น member หรือไม่
    bool first_play = true;//ตัวแปรสำหรับเช็คว่าเป็นรอบแรกที่เล่นหรือไม่
    FILE *data;

    //โหลดข้อมูลผู้เล่นและคะแนนจากไฟล์ มาเปรียบเทียบว่าเป็นผู้เล่นเก่าหรือใหม่
    data = fopen("data.txt","r");
    fscanf(data,"%d",&num_data);
    //เช็คว่ามีชื่อที่กรอกมาอยู่ในไฟล์หรือไม่  -  O(n)
    while(fscanf(data,"%s %d %d",loaded_name,&loaded_pscore,&loaded_cscore) != EOF){
        if(strcmp(loaded_name,name) == 0){
            member = true;
            player_score = loaded_pscore;
            computer_score = loaded_cscore;
            break;
        }
    }
    if(member){//ถ้าเคยเป็น member โชว์คะแนนที่บันทึกไว้
        printf("Welcome back %s, your score is %d and computer's score is %d\n",name,player_score,computer_score);
        global_seed += player_score;// เปลี่ยน seed เพื่อให้ไม่เจอการ์ดชุดเดิม
    }else{
        //printf("text for new player");
    }
    fclose(data);
//==============================================================


    //MAIN LOOP
    while(playing){
        //ตัวแปรสำหรับเล่นเกมแต่ละเกม
        deck_size = 52;
        player_size = 0; //size of player's deck
        computer_size = 0; //size of computer's deck
        char input[25];

        if(!first_play){//เช็คว่าเป็นรอบแรกในการเล่นหรือเปล่า ถ้าไม่หมายถึงเล่นจบมาแล้วอย่างน้อยหนึ่งเกม
            while(1){
                //ระบบถามว่าจะทำอะไรต่อ P เล่นต่อโดยสับเด็ค T เล่นต่อโดยไม่สับเด็ค Q ออก
                printf("type (P)lay for shuffle, type (T)ry again for continue without shuffle, or type (Q)uit for stop.\n-> ");
                scanf("%s",input);

                if(strcmp(input,"Q") == 0 || strcmp(input,"q") == 0){
                    //อัปเดตคะแนน และหยุดเล่น
                    update(name,player_score,computer_score,member);
                    playing = false;
                    printf("Thank you for using the service. See you again next time.");
                
                }else if(strcmp(input,"P") == 0 || strcmp(input,"p") == 0){
                    //สับเด็ค
                    shuffle(deck,deck_size);
                    printf("\nThe deck of cards has been shuffled.\n");
                
                }else if(strcmp(input,"T") == 0 || strcmp(input,"t") == 0){
                    //ไม่ทำอะไร    
                }else{
                    continue;
                }
                break;
            }
        }else{
            shuffle(deck,deck_size);
            printf("\nThe deck of cards has been shuffled.\n");
        }

        //ออกจาก main loop ถ้าเลิกเล่นแล้ว
        if(!playing){break;}


        printf("Deal the cards.\n");
        int starter = 2; //จำนวนการ์ดเริ่ม
        while(starter--){
            //แจกการ์ดเริ่มต้น
            add_card(player_deck,&player_size,pop_card(deck,&deck_size));
            add_card(computer_deck,&computer_size,pop_card(deck,&deck_size));
        }
        //แสดงการ์ดและคะแนนที่มีเริ่มต้นของทั้งสองฝั่ง
        display(player_deck,computer_deck,name,"Computer",player_size,computer_size);
        strcpy(input,"");
        

        //player's turn
        while(score(player_deck,player_size) < goal){ //จะจบเทิร์นผู้เล่นก็ต่อเมื่อ ผู้เล่น Stand หรือ คะแนน >= 21
            printf("%s, would you (H)it or (S)tand..? -> ",name);//อ่านค่าว่าผู้เล่นจะ Hit หรือ Stand
            scanf("%s",input);
            if(strcmp(input,"s") == 0 || strcmp(input,"S") == 0){//ถ้า Stand จบเทิร์นผู้เล่น ไปยังเทิร์นคอมพิวเตอร์
                break;
            }else if(strcmp(input,"h") != 0 && strcmp(input,"H") != 0){//ถ้าเป็นอย่างอื่นที่ไม่ใช่ Hit หรือ Stand รับค่าใหม่
                continue;
            }else{
                add_card(player_deck,&player_size,pop_card(deck,&deck_size));
                display(player_deck,computer_deck,name,"Computer",player_size,computer_size);//หลังจาก Hit จะแสดงการ์ดและคะแนนของทั้งสองฝั่ง
            }
        }
        int pscore = score(player_deck,player_size); //ตัวแปรเก็บแต้มผู้เล่น จากการใช้ฟังก์ชั่น score()


        //computer's turn
        //คอมพิวเตอร์จะ Hit ก็ต่อเมื่อแต้มน้อยกว่าผู้เล่น และ แต้มผู้เล่นไม่เกิน 21 ไม่งั้นจะ Stand 
        printf("\nNow,it's my turn...\n");
        display(player_deck,computer_deck,name,"Computer",player_size,computer_size);
        while(score(computer_deck,computer_size) < pscore && pscore <= goal){
            printf("Computer -> Hit\n");
            add_card(computer_deck,&computer_size,pop_card(deck,&deck_size));
            display(player_deck,computer_deck,name,"Computer",player_size,computer_size);//หลังจาก Hit จะแสดงการ์ดและคะแนนของทั้งสองฝั่ง
        }
        printf("Computer -> Stand\n");
        int cscore = score(computer_deck,computer_size);//ตัวแปรเก็บแต้มคอมพิวเตอร์ จากการใช้ฟังก์ชั่น score()


        //สรุปผล
        printf("\nWHO WIN?\n");
        if(pscore > goal){
            printf("[Computer] wins this round!");
            computer_score++;
        }else if(cscore > goal){
            printf("[%s] wins this round!",name);
            player_score++;
        }else if(pscore > cscore){
            printf("[%s] wins this round!",name);
            player_score++;
        }else if(cscore > pscore){
            printf("[Computer] wins this round!");
            computer_score++;
        }else if(cscore == pscore){
            printf("Draw..");
        }
        printf("\n\n[%s] %d : %d [%s]\nDo you want to continue playing?\n",name,player_score,computer_score,"COMPUTER");
        first_play = false;
    }

    return 0;
}