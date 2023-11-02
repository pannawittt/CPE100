#include <stdio.h>
#include <string.h>
unsigned int global_seed = 1022;
int goal = 21; //goal to win

typedef struct{
    char name[100];
    int score;
} player;

typedef enum{
    true=1,
    false=0
} bool;

unsigned int random(unsigned int seed,int range){
    global_seed += (9*seed+173)%range;
    global_seed = global_seed % 4000009;
    return (9*seed+173)%range;
}

void shuffle(char* deck[],int size){
    while(size--){
        int i = random(global_seed,size+1);
        int j = size;
        char* temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void straight_print(char deck[][5],int index){ // 7-♥ J-♣ A-♣  
    if(index != 1){
        straight_print(deck,index-1);
    }
    printf("%s ",deck[index-1]);
}

int score(char deck[][5],int size){
    int sum=0;
    int ace=0;
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
        if(sum > goal && ace>0){
            sum-=9;
            ace--;
        }
    }
    return sum;
}

void display(char deck1[][5],char deck2[][5],char* name1,char* name2,int size1,int size2){
    printf("   %s: ",name1);
    straight_print(deck1,size1);
    printf("(%d)",score(deck1,size1));
    printf("     %s: ",name2);
    straight_print(deck2,size2);
    printf("(%d)",score(deck2,size2)); 
    printf("\n");
}

char* pop_card(char* deck[],int* size){
    *size -= 1;
    return deck[*size];
}

void add_card(char deck[][5],int* size,char* card){
    strcpy(deck[*size],card);
    *size += 1;
}

void print_deck(char* deck[],int size){
    printf("\n   ");
    for(int i=0;i<size;i++){
        if(i%13==0 && i!=0){printf("\n\n   ");}
        printf(" %s ",deck[i]);
    }
    printf("\n\n");
}

void seed_generator(char str[]){
    int i = 0;
    while(1){
        if(str[i] == '\0'){break;}
        global_seed *= (int)str[i];
        global_seed = global_seed% 400009;
        i++;
    }
}

void update(char* nme,int scr,bool ismember){
    FILE *data_input = fopen("data.txt","r");
    int num_data;
    int member_index;
    fscanf(data_input,"%d",&num_data);
    if(!ismember){
        num_data++;
    }
    player player_data[num_data];
    int index=0;
    while(fscanf(data_input,"%s %d",player_data[index].name,&player_data[index].score) != EOF){
        if(strcmp(player_data[index].name,nme) == 0){
            member_index = index;
        }
        index++;
    }
    if(!ismember){
        strcpy(player_data[index].name,nme);
        player_data[index].score = scr;
    }else{
        player_data[member_index].score = scr;
    }
    char output[2000]="";
    sprintf(output,"%d\n",num_data);
    for(int i=0;i<num_data;i++){
        char tmp[150];
        sprintf(tmp,"%s %d",player_data[i].name,player_data[i].score);
        strcat(output,tmp);
        if(i != num_data-1){strcat(output,"\n");}
    }
    fclose(data_input);
    FILE *data_output = fopen("data.txt","w");
    fprintf(data_output,output);
    fclose(data_output);
}

int main(){
    char* deck[] = {"A-\3","2-\3","3-\3","4-\3","5-\3","6-\3","7-\3","8-\3","9-\3","10-\3","J-\3","Q-\3","K-\3",
                    "A-\4","2-\4","4-\4","4-\4","5-\4","6-\4","7-\4","8-\4","9-\4","10-\4","J-\4","Q-\4","K-\4",
                    "A-\5","2-\5","4-\5","4-\5","5-\5","6-\5","7-\5","8-\5","9-\5","10-\5","J-\5","Q-\5","K-\5",
                    "A-\6","2-\6","4-\6","4-\6","5-\6","6-\6","7-\6","8-\6","9-\6","10-\6","J-\6","Q-\6","K-\6"};
    int deck_size = 52;
    char name[100];
    int player_score = 0;
    int computer_score = 0;
    char player_deck[10][5];
    int player_size=0;
    char computer_deck[10][5];
    int computer_size=0;
    bool playing = true;

    printf("==========================================================================\n\n");
    printf("    Welcome to Blackjack! This is the card deck for playing this game\n");
    print_deck(deck,deck_size);
    printf("==========================================================================\n\n");

    printf("Please enter your name.\n-> ");
    scanf("%s",name);
    seed_generator(name);
    
    char input_name[100];
    int input_score;
    int num_data;
    bool member = false;
    bool first_play = true;
    FILE *data;

    data = fopen("data.txt","r");
    fscanf(data,"%d",&num_data);
    while(fscanf(data,"%s %d",input_name,&input_score) != EOF){
        if(strcmp(input_name,name) == 0){
            member = true;
            player_score = input_score;
            break;
        }
    }
    if(member){
        printf("Welcome back %s, your score is %d\n",name,player_score);
        global_seed += player_score;
    }else{
       
    }
    fclose(data);

    while(playing){
        deck_size = 52;
        player_size = 0; //size of player's deck
        computer_size = 0; //size of computer's deck
        char input[25];
        unsigned int seed;
        if(!first_play){
            while(1){
                printf("type (P)lay for shuffle, type (T)ry again for continue without shuffle, or type (Q)uit for stop.\n-> ");
                scanf("%s",input);
                if(strcmp(input,"Q") == 0 || strcmp(input,"q") == 0){
                    update(name,player_score,member);
                    playing = false;
                    printf("Thank you for using the service. See you again next time.");
                }else if(strcmp(input,"P") == 0 || strcmp(input,"p") == 0){
                    shuffle(deck,deck_size);
                    printf("\nThe deck of cards has been shuffled.\n");
                }else if(strcmp(input,"T") == 0 || strcmp(input,"t") == 0){
                    
                }else{
                    continue;
                }
                break;
            }
        }else{
            shuffle(deck,deck_size);
            printf("\nThe deck of cards has been shuffled.\n");
        }
        if(!playing){break;}
        printf("Deal the cards.\n");
        int starter = 2; //จำนวนการ์ดเริ่ม
        while(starter--){
            add_card(player_deck,&player_size,pop_card(deck,&deck_size));
            add_card(computer_deck,&computer_size,pop_card(deck,&deck_size));
        }
        display(player_deck,computer_deck,name,"Computer",player_size,computer_size);
        strcpy(input,"");
        
        //ตาผู้เล่น
        while(score(player_deck,player_size) < goal){
            printf("%s, would you (H)it or (S)tand..? -> ",name);
            scanf("%s",input);
            if(strcmp(input,"s") == 0 || strcmp(input,"S") == 0){
                break;
            }else if(strcmp(input,"h") != 0 && strcmp(input,"H") != 0){
                continue;
            }else{
                add_card(player_deck,&player_size,pop_card(deck,&deck_size));
                display(player_deck,computer_deck,name,"Computer",player_size,computer_size);
            }
        }
        int pscore = score(player_deck,player_size);

        //ตาบอท
        printf("\nNow,it's my turn...\n");
        display(player_deck,computer_deck,name,"Computer",player_size,computer_size);
        while(score(computer_deck,computer_size) < pscore && pscore <= goal){
            printf("Computer -> Hit\n");
            add_card(computer_deck,&computer_size,pop_card(deck,&deck_size));
            display(player_deck,computer_deck,name,"Computer",player_size,computer_size);
        }
        printf("Computer -> Stand\n");
        int cscore = score(computer_deck,computer_size);

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