#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "rbc.c"

#define CHECK_ERROR(val1,val2,msg)   if (val1==val2) \
                                    { perror(msg); \
                                        exit(EXIT_FAILURE); }

// To use : ./server <ip> <port>

#define MAXOCTETS   150

int init_socket(char * ip_addr, int port, struct sockaddr_in * adrlect){
    int sd = socket(AF_INET, SOCK_DGRAM, 0);
    CHECK_ERROR(sd, -1, "Erreur socket non cree !!! \n");
    printf("N° de la socket : %d \n", sd);
    printf("Adrlect %d port %d\n", inet_addr(ip_addr), htons(port));

    adrlect->sin_family = AF_INET;
    adrlect->sin_port = htons(port);
    adrlect->sin_addr.s_addr = inet_addr(ip_addr);

    int erreur=bind(sd, (const struct sockaddr *) adrlect, sizeof(*adrlect));
    CHECK_ERROR(erreur, -1, "Erreur de bind !!! \n");

    return sd;
}

void scan_msg(char * buffer, struct sockaddr_in * adrecriv, int * sd){
    char buff_emission[MAXOCTETS+1];
    int nbcar;
    socklen_t adrecriv_len;
    adrecriv_len = sizeof(adrecriv);
    //reception du message de la part de l'écrivain
    nbcar=recvfrom(*sd, buffer, MAXOCTETS+1, 0, (struct sockaddr * ) adrecriv, &adrecriv_len);
    CHECK_ERROR(nbcar, 0, "\nProbleme de reception !!! \n");
    printf("MSG RECU DU CLIENT ADRESSE IP %s PORT %d > %s\n", inet_ntoa(adrecriv->sin_addr), adrecriv->sin_port, buffer);

    //vérification de la demande de déconnexion
    if (strcmp(buff_emission, "exit") == 0 || strcmp(buffer, "exit") == 0){
        CHECK_ERROR(close(*sd), -1, "Erreur lors de la fermeture de la socket");
        return;
    }
}

void answer_msg(int * sd, struct sockaddr_in * adrecriv, char * msg){
    socklen_t adrecriv_len;
    adrecriv_len = sizeof(struct sockaddr_in);

    sendto(*sd,msg, strlen(msg), 0,(struct sockaddr *) adrecriv, adrecriv_len);
    printf("MSG ENVOYE AVEC SOCKET %d AU CLIENT ADRESSE IP %s PORT %d > %s\n", *sd, inet_ntoa(adrecriv->sin_addr), adrecriv->sin_port, msg);
}

int main(int argc, char * argv[]) {
    if (argc < 3){
        printf("Missing arguments. Use ./server <ip> <port>\n");
        exit(0);
    }
    char * local_ip = argv[1];
    int local_port = atoi(argv[2]);
    printf("%s, %d\n", local_ip, local_port);
    int sd;
    int erreur;
    struct sockaddr_in adrlect;
    struct sockaddr_in adrecriv;

    sd = init_socket(local_ip, local_port, &adrlect);

    printf("Adrlect %d port %d\n", adrlect.sin_addr.s_addr, adrlect.sin_port);
    
    trainData trainRegister[TRACKLENGTH];
    
    init_register(trainRegister);
    
    while (1){
        char buffer[MAXOCTETS+1] = {""};

        scan_msg(buffer, &adrecriv, &sd);
        
        int result = parse_msg(trainRegister, buffer);

        char * msg = (result ? "1" : "0");
        printf("%s\n", msg);

        answer_msg(&sd, &adrecriv, msg);
    
        display_trains(trainRegister);

    }
    exit(EXIT_SUCCESS);
}