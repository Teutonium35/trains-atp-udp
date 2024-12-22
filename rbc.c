#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define TRACKLENGTH 1000

typedef struct{
    char no_train[20];
    int loc;
    int eoa;
} trainData;

trainData empty_train;

/*
Procedures to call :
id 1 : add train, data = no_train, server answers acknowledge
id 2 : remove train, data = no_train, servers answers acknowledfe
id 3 : ask permission, data = no_train, destination, servers answers yes no
id 4 : move train, data = no_train, destination, server answers acknowledge


*/

int add_train(trainData trainRegister[], char * no_train){
    if ((trainRegister[0].loc == 0) && (trainRegister[0].no_train[0] != 0)) {
        printf("Cannot add a train : there is already %s at loc 0 !\n", trainRegister[0].no_train);
        return 0;
    }
    for(int i = 1;i<TRACKLENGTH;i++){
        trainRegister[TRACKLENGTH-i] = trainRegister[TRACKLENGTH-(i+1)];
    }
    trainData new_train;
    strcpy(new_train.no_train, no_train);
    new_train.loc = 0;
    new_train.eoa = 0;
    trainRegister[0] = new_train;
    if (trainRegister[1].loc != -1){
        trainRegister[0].eoa = trainRegister[1].loc - 1;
    }
    else{
        trainRegister[0].eoa = TRACKLENGTH;
    }
    return 1;
}

int remove_train(trainData trainRegister[], char * no_train){
    for(int i = 0;i<TRACKLENGTH;i++){
        if ((trainRegister[i].loc == TRACKLENGTH) && (!strcmp(trainRegister[i].no_train, no_train))){
            trainRegister[i] = empty_train;
        }
    }
    return 1;
}

void display_trains(trainData trainRegister[]){
    for (int i = 0;i<TRACKLENGTH;i++){
        if (trainRegister[i].no_train[0] != 0){
            printf("Train no %s at loc %d and eoa %d\n", trainRegister[i].no_train, trainRegister[i].loc, trainRegister[i].eoa);
        }
    }
}

void init_register(trainData trainRegister[]){
    strcpy(empty_train.no_train, "");
    empty_train.loc = -1;
    empty_train.eoa = -1;
    for (int i = 0;i<TRACKLENGTH;i++){
        trainRegister[i] = empty_train;
    }
}

int ask_permission(trainData trainRegister[], char * no_train, int new_location){
    int i = 0;
    while (strcmp(trainRegister[i].no_train, no_train) && i < TRACKLENGTH){
        i++;
    }
    if (i == TRACKLENGTH){
        printf("Did not find train %s\n", no_train);
        return 0;
    }
    else{
        if (new_location > trainRegister[i].eoa){
            return 0;
        }
        return 1;
    }
}

int move_train(trainData trainRegister[], char * no_train, int new_location){
    int i = 0;
    while (strcmp(trainRegister[i].no_train, no_train) && i < TRACKLENGTH){
        i++;
    }
    int old_location = trainRegister[i].loc;

    if (i == TRACKLENGTH){
        printf("Did not find train %s here\n", no_train);
    }
    else{
        if (!ask_permission(trainRegister, no_train, new_location)){
            printf("Did not get authorization to move train %s\n", trainRegister[i].no_train);
            return 0;
        }
        trainRegister[i].loc = new_location;
        printf("Successfully moved train %s from %d to %d\n", trainRegister[i].no_train, old_location, trainRegister[i].loc);
        if (i!=0){
            trainRegister[i-1].eoa = new_location - 1;
        }
    }
    return 1;
}

int parse_msg(trainData trainRegister[], char * msg){
    char name_buffer[10] = {""};
    int i = 0;
    while (i<strlen(msg) && msg[i] != ':'){
        name_buffer[i] = msg[i];
        i++;
    }
    i++;
    int procedure_to_call = msg[i] - '0';
    i += 2;
    char data_buffer[20] = {""};
    int j = 0;
    while(msg[i] != 0){
        data_buffer[j] = msg[i];
        i++;
        j++;
    }

    printf("Decoded message %s for train %s, procedure %d (%c), with data %s\n", msg, name_buffer, procedure_to_call, procedure_to_call, data_buffer);

    if (procedure_to_call == 1){
        return add_train(trainRegister, name_buffer);
        // TODO : answer ack
    }
    else if (procedure_to_call == 2){
        return remove_train(trainRegister, name_buffer);
        // TODO : answer ack
    }
    else if (procedure_to_call == 3){
        char end_buffer[10] = "";
        j = 0;
        i = 0;
        while(data_buffer[i] != ':'){
            end_buffer[j] = data_buffer[i];
            j++;
            i++;
        }
        printf("Ask permission to move %s to %s\n", name_buffer, end_buffer);
        return ask_permission(trainRegister, name_buffer, atoi(end_buffer));
    }
    else if (procedure_to_call == 4){
        char end_buffer[10] = "";
        j = 0;
        i = 0;
        while(data_buffer[i] != ':'){
            end_buffer[j] = data_buffer[i];
            j++;
            i++;
        }
        printf("Move %s to %s\n", name_buffer, end_buffer);
        return move_train(trainRegister, name_buffer, atoi(end_buffer));
    }
}

void format_add_request(char * no_train, char msg[50]){
    sprintf(msg, "%s:1:", no_train);
    printf("%s\n",msg);
}

void format_permission_request(char * no_train, char msg[50], int destination){
    sprintf(msg, "%s:3:%d:", no_train, destination);
    printf("%s\n",msg);
}

void format_movement_request(char * no_train, char msg[50], int destination){
    sprintf(msg, "%s:4:%d:", no_train, destination);
    printf("%s\n",msg);
}

void format_remove_request(char * no_train, char msg[50]){
    sprintf(msg, "%s:2:", no_train);
    printf("%s\n",msg);
}



// int main(){
//     char msg[50];
//     format_add_request("TGV0", msg);
//     printf("%s\n", msg);
// }