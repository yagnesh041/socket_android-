// A synchronized client/server message exchange(1 to 1).

// Example 2: Client

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
//	char message[255];
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
				game();
				break;
			case 2:
				showBalance(balance);
				break;
			case 3:
				printf("rules\n");
				break;
			case 4:
				printf("Hope you liked the game...Bye Bye...\n");
				close(server);
				exit(0);
				break;
			default:
				printf("Enter valid choice\n");
				break;
		}




/*		if(read(server, message, 255)<0)
		{
			fprintf(stderr, "read() error\n");
			exit(3);
		}

		fprintf(stderr, "Server’s messgae: %s\n",message);
		fprintf(stderr, "Enter the $ sign to quit or a message for the server\n");
		
		fgets(message, 254, stdin);
		if(message[0] == '$')
		{
			close(server);
			exit(0);
		}

		write(server, message, strlen(message)+1);*/
	}
	return 0;
}

void showBalance(int bal)
{
	printf("your current balance is : $%d\n",bal);	
}

void game()
{
	valid:
	printf("Enter your lucky integer number from 1-50:");
	scanf("%d",&userNumber);
	printf("Enter your bet from $5 to $50 and less than your balance:");
	scanf("%d",&bet);
	if(userNumber>=1 && userNumber<=50 && bet>=5 && bet<=50 && bet<=balance)
	{
		balance = balance - bet;
		write(server, &userNumber, sizeof(userNumber));
		write(server, &bet, sizeof(bet));
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
			balance = balance + price;
			printf("Dealer's number is : %d\n",dealerNumber);
			printf("you won $%d\n",price);
		}
				
	}
	else if(balance<5)
	{
		printf("sorry you are out of balance so you lost the game try again next time...\n");
		close(server);
		exit(0);	
	}
	else
	{
		printf("you entered wrong input\n");
		goto valid;
	}
}
