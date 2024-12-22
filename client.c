#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "rbc.c"

#define CHECK_ERROR(val1, val2, msg)	if (val1==val2) { printf("%s \n", msg) ; exit(EXIT_FAILURE); }
#define MAXCARS	80

int main (int argc, char * argv[])
{
    if (argc<4){
        printf("Missing arguments. Use ./client <ipserv> <portserv> <iplocal>\n");
        exit(0);
    }
    int sd; //socket de dialogue
    struct sockaddr_in addr_evc, addr_rbc;
    int longaddr;
    int nbcar;

    int code_erreur;

    char recept[MAXCARS+1]; //buffer de reception
    char emiss[MAXCARS+1]; //buffer d'emission

    //Etape 1 : creation de la socket
    sd=socket(AF_INET, SOCK_DGRAM, 0); //creation d'un descripteur de socket pour communiquer avec des processus localises sur la même machine.
    CHECK_ERROR(sd,-1, "la creation de la socket");

    //Etape 2 : Je definis l'adresse de la socket

    addr_evc.sin_family=AF_INET;
    //addr_evc.sin_port = 2000; 
    addr_rbc.sin_family=AF_INET;

    addr_evc.sin_addr.s_addr = inet_addr(argv[3]);

    if (argc < 3){
        printf("usage %s <adresse ip rbc> <port rbc>", argv[0]); 
        return EXIT_FAILURE; 
    }
    else{
        addr_rbc.sin_addr.s_addr = inet_addr (argv[1]);
        
        u_int16_t port  = htons(atoi(argv[2])); 
        addr_rbc.sin_port = port; 
        printf("adresse ip : %s\n", inet_ntoa(addr_rbc.sin_addr)); 
    }

    //Etape 3 : affectation de l'adresse à la socket

    code_erreur = bind(sd, (struct sockaddr *)&addr_evc, sizeof(addr_evc));
    CHECK_ERROR(code_erreur,-1,"bind");
    char instruction_set[5][50];
    format_add_request("TGV0", instruction_set[0]);
    printf("Test\n");
    format_permission_request("TGV0", instruction_set[1], 150);
    format_movement_request("TGV0", instruction_set[2], 150);
    format_movement_request("TGV0", instruction_set[3], 1000);
    format_remove_request("TGV0", instruction_set[4]);
    
    //Etape 4 : emission d'une reponse
    // printf("evc> ");

    longaddr=sizeof(struct sockaddr_in);

    // fgets(emiss,MAXCARS,stdin);
    // emiss[strlen(emiss)-1]='\0'; //suppression du retour chariot qui est insere dans le buffer par fgets
    // format_add_request("TGV0", emiss);

    for (int i = 0;i<5; i++){
        sleep(1);
        nbcar=sendto(sd,instruction_set[i], strlen(instruction_set[i]), 0,(struct sockaddr *) &addr_rbc, longaddr);

        if (nbcar < 0) {
            perror("Erreur lors de l'envoi redacteur");
            exit(0); // ou exit(EXIT_FAILURE);
        }

        //Etape 5 : reception de data
        nbcar=recvfrom(sd,recept,MAXCARS+1, 0, (struct sockaddr *) &addr_rbc, &longaddr);

        if (nbcar > 0) {
                recept[nbcar] = '\0'; // Terminer la chaîne reçue
                printf("RBC IP:%s> '%s'\n", inet_ntoa(addr_rbc.sin_addr),recept);          
            }
        //while (strcmp(emiss,"end"));
    }

    CHECK_ERROR(close(sd), -1, "Erreur lors de la fermeture de la socket");
    exit(EXIT_SUCCESS);
}
