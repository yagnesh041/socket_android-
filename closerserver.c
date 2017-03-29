/* Project Id: 17 */

/* server code for Closer - A Number to Win */

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>

void child(int sd);

int main(int argc, char *argv[])
{
        int sd, client, portNumber;
        socklen_t len;
        struct sockaddr_in servAdd;

        if(argc != 2)
        {
                printf("Call model: %s <Port #>\n", argv[0]);
                exit(0);
        }

        if ((sd=socket(AF_INET,SOCK_STREAM,0))<0)
        {
                fprintf(stderr, "Cannot create socket\n");
                exit(1);
        }

        servAdd.sin_family = AF_INET;
        servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
        sscanf(argv[1], "%d", &portNumber);
        servAdd.sin_port = htons((uint16_t)portNumber);
        bind(sd,(struct sockaddr*)&servAdd,sizeof(servAdd));
        listen(sd, 5);

        while(1)
        {
                client=accept(sd,(struct sockaddr*)NULL,NULL);
                printf("Got a client\n");
                if(!fork())
                        child(client);
                close(client);
        }
}

void child(int sd)
{
        int userNumber,bet,dealerNumber,difference,price;
        while(1)
        {
                /* check if username is properly read or not */
                if(!read(sd, &userNumber, sizeof(userNumber)))
                {
                        close(sd);
                        printf("problem in reading user's number or player quit the game\n");
                        exit(0);
                }
                /* check if bet is properly read or not */
                else if(!read(sd, &bet, sizeof(bet)))
                {
                        close(sd);
                        printf("problem in reading bet or player quit the game\n");
                        exit(0);
                }
                else
                {
                        /* generate random number between 1 and 50 for dealer */
                        dealerNumber = rand()%50;
                        printf("dealer generated number is : %d\n",dealerNumber);

                        /* calculate difference between usernumber and dealernumber */
                        difference = abs(userNumber - dealerNumber);
                        printf("difference between User's and Dealer's number is : %d\n",difference);

                        /* calculate price based on difference */
                        if(difference==0)
                        {
                                price = bet * 10;
                        }
                        else if(difference == 1)
                        {
                                price = bet * 5;
                        }
                        else if(difference == 2)
                        {
                                price = bet * 4;
                        }
                        else if(difference == 3)
                        {
                                price = bet * 3;
                        }
                        else if(difference == 4)
                        {
                                price = bet * 2;
                        }
                        else if(difference == 5)
                        {
                                price = bet * 1;
                        }
                        else
                        {
                                price = 0;
                        }

                        /* send dealerNumber and price to client */
                        write(sd, &dealerNumber, sizeof(dealerNumber));
                        write(sd, &price, sizeof(price));
                }
        }
}

