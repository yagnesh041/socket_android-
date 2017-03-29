/* Project Id: 17 */

/* client code for Closer - A Number to Win */

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>

int balance=500,userNumber,bet,dealerNumber,price;
int server,portNumber;

void showBalance(int bal);
void game();
int main(int argc, char *argv[])
{
        int choice;
        socklen_t len;
        struct sockaddr_in servAdd;

        if(argc != 3)
        {
                printf("Call model:%s <IP> <Port#>\n",argv[0]);
                exit(0);
        }

        if((server = socket(AF_INET, SOCK_STREAM, 0))<0)
        {
                fprintf(stderr, "Cannot create socket\n");
                exit(1);
        }

        servAdd.sin_family = AF_INET;
        sscanf(argv[2], "%d", &portNumber);
        servAdd.sin_port = htons((uint16_t)portNumber);

        if(inet_pton(AF_INET,argv[1],&servAdd.sin_addr)<0)
        {
                fprintf(stderr, " inet_pton() has failed\n");
                exit(2);
        }

        if(connect(server,(struct sockaddr *)&servAdd,sizeof(servAdd))<0)
        {
                fprintf(stderr, "connect() has failed, exiting\n");
                exit(3);
        }

        while(1)
        {
                /* Display Menu */
                printf("==================================================\n");
                printf("1. Play Game\n");
                printf("2. Display Balance\n");
                printf("3. Display Rules and game instructions\n");
                printf("4. Quit the game\n");
                printf("Enter your choice:");
                scanf("%d",&choice);
                switch(choice)
                {
                        case 1:
                                /* play game */
                                game();
                                break;
                        case 2:
                                /* display balance */
                                showBalance(balance);
                                break;
                        case 3:
                                /* rules of the game */
                                printf("1: Enter valid integer number of your choice from 1-50\n");
                                printf("2: Put bet between $5-$50 on your number\n");
                                printf("3: your number is compared with dealer's number and you will win according to how closer your number is from delaer's number\n");
                                printf("difference=0 ::: you will get (10 x bet)\n");
                                printf("difference=1 ::: you will get (5 x bet)\n");
                                printf("difference=2 ::: you will get (4 x bet)\n");
                                printf("difference=3 ::: you will get (3 x bet)\n");
                                printf("difference=4 ::: you will get (2 x bet)\n");
                                printf("difference=5 ::: you will get (1 x bet)\n");
                                printf("difference>5 ::: you will get (0 x bet) or you lost the bet\n");
                                break;
                        case 4:
                                /* exit statement */
                                printf("Hope you liked the game...Bye Bye...\n");
                                close(server);
                                exit(0);
                                break;
                        default:
                                printf("Enter valid choice\n");
                                break;
                }
        }
        return 0;
}

/* Display remaining balance */
void showBalance(int bal)
{
        printf("your current balance is : $%d\n",bal);
}

void game()
{
        valid:
        /* take usernumber and bet from user */
        printf("Enter your lucky integer number from 1-50:");
        scanf("%d",&userNumber);
        printf("Enter your bet from $5 to $50 and less than your balance:");
        scanf("%d",&bet);
        /* check for valid usernumber and bet */
        if(userNumber>=1 && userNumber<=50 && bet>=5 && bet<=50 && bet<=balance)
        {
                /* subtract bet from balance */
                balance = balance - bet;
                /* send usernumber and bet to server for computation */
                write(server, &userNumber, sizeof(userNumber));
                write(server, &bet, sizeof(bet));
                /* read dealernumber and price from server */
                if(!read(server, &dealerNumber, sizeof(dealerNumber)))
                {
                        printf("problem reading dealer number from server...try again");
                        goto valid;
                }
                else if(!read(server, &price, sizeof(price)))
                {
                        printf("problem reading price from server... try again");
                        goto valid;
                }
                else
                {
                        /* update balance using price obtain from server */
                        balance = balance + price;
                        /* print dealernumber and price on client side */
                        printf("Dealer's number is : %d\n",dealerNumber);
                        printf("you won $%d\n",price);
                }

        }
        /* check if user is out of balance or not */
        else if(balance<5)
        {
                printf("sorry you are out of balance so you lost the game try again next time...\n");
                close(server);
                exit(0);
        }
        else
        {
                /* if user input worng data then go back to inserting data using goto function */
                printf("you entered wrong input\n");
                goto valid;
        }
}
