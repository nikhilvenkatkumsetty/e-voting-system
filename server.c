#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<my_global.h>
#include<mysql.h>
#include<math.h>
#include<time.h>

long int p,q,n,t,flag,e[100],d[100],temp[100],j,i;
char en[100];
char m[100];
char msg[100];
int prime(long int);
void ce();
long int cd(long int);
void encrypt();
void decrypt();

void error(const char * msg)
{
	perror(msg);
	exit(1);
}

void finish_with_error(MYSQL *con)
{
	fprintf(stderr, "%s\n", mysql_error(con));
	mysql_close(con);
	exit(1);
}
int main(int argc,char * argv[])
{
	if(argc<2)
	{
		fprintf(stderr,"port number not specified program terminated\n");
		exit(1);
	}
	MYSQL *con = mysql_init(NULL);
	char query[200];
	int sockfd, newsockfd, portno,check;
	char buffer[255];
	struct sockaddr_in serv_addr,cli_addr;
	socklen_t clilen;
	char userid[50],vote[50];
	char aadhar[50];
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		error("Socket not created\n");
	}
	bzero((char*)&serv_addr,sizeof(serv_addr));
	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if(bind(sockfd , (struct sockaddr*)&serv_addr , sizeof(serv_addr))<0)
	{
		error("Binding failed\n");
	}
	listen(sockfd,5);
	if(con==NULL)
        {
                fprintf(stderr, "%s\n", mysql_error(con));
                exit(1);
        }
        if (mysql_real_connect(con, "localhost", "ccnteam", "Ccn123!@#","pollbooth", 0, NULL, 0) == NULL)
        {
                finish_with_error(con);
        }

	time_t start, end;
	double elapsed;  // seconds
	start = time(NULL);
	int terminate = 1;
	while(terminate)
	{

		end = time(NULL);
     		elapsed = difftime(end, start);
     		if (elapsed >= 60.0 /* seconds */)
       		terminate = 0;
     		else
     		{
			clilen = sizeof(cli_addr);
			newsockfd = accept(sockfd,(struct sockaddr*)&cli_addr , &clilen);
			if(newsockfd<0)
				error("error in accept");
			printf("Connection accepted from %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
			bzero(buffer,255);//emptying the buffer
			check=read(newsockfd,buffer,255);
			if(check<0)
				error("error in reading\n");
			strcpy(userid,buffer);
			bzero(buffer,255);

			strcpy(buffer,"userid received");
			check=write(newsockfd,buffer,strlen(buffer));
			if(check<0)
				error("error in writing\n");

			bzero(buffer,255);
			check=read(newsockfd,buffer,255);
			if(check<0)
				error("error in reading\n");
			strcpy(aadhar,buffer);


	
	int flag=0;
	if (mysql_query(con, "Select * FROM Voting"))
  {
      finish_with_error(con);
  }
	MYSQL_RES * chck = mysql_store_result(con);
	if(chck==NULL)
	{
		printf("error\n");
		finish_with_error(con);
	}
	int num_fields = mysql_num_fields(chck);
	MYSQL_ROW row;
	while((row = mysql_fetch_row(chck)))
	{
		for(int i=0;i< num_fields ; i++)
		{
			if(strcmp(row[1],aadhar)==0)
			flag=1;
			break;
		}
	}

	if(flag==1)
	{
		bzero(buffer,255);
//		strcpy(buffer,"you have already casted the vote!");
		strcpy(buffer,"1");
		check=write(newsockfd,buffer,strlen(buffer));
        if(check<0)
                error("error in writing\n");
	continue;

	}



			bzero(buffer,255);

			strcpy(buffer,"aadhar received");
			check=write(newsockfd,buffer,strlen(buffer));
			if(check<0)
				error("error in writing\n");

			bzero(buffer,255);
  			check=read(newsockfd,buffer,255);
			if(check<0)
				error("error in reading\n");
			strcpy(vote,buffer);
			strcpy(en,vote);
  			bzero(buffer,255);
			strcpy(buffer,"vote received");
			check=write(newsockfd,buffer,strlen(buffer));
			if(check<0)
  				error("error in writing\n");

			for(i=0;i<strlen(vote);i++)
			{
				bzero(buffer,255);
				check=read(newsockfd,buffer,255);
				if(check<0)
					error("error in reading\n");
				int var = atoi(buffer);
				temp[i]=var;
				check=write(newsockfd,buffer,strlen(buffer));
				if(check<0)
					error("error in writing\n");
				bzero(buffer,255);
			}

			check=read(newsockfd,buffer,255);
			if(check<0)
				error("error in reading\n");
			d[0]=atoi(buffer);
			bzero(buffer,255);
			decrypt();

			sprintf((char*)query,"INSERT INTO Voting VALUES ('%s','%s','%s')",userid,aadhar,m);
			if(mysql_query(con,(const char*)query))
			{
				finish_with_error(con);
			}
     		}

	}

	int count[8]={0};
	if (mysql_query(con, "Select * FROM Voting"))
  	{
      		finish_with_error(con);
  	}
        MYSQL_RES *result = mysql_store_result(con);
        if (result == NULL)
  	{
          	printf("error\n");
      		finish_with_error(con);
  	}

  	int num_fields = mysql_num_fields(result);
  	MYSQL_ROW row;
 	while ((row = mysql_fetch_row(result)))
  	{
	  	if((strcmp(row[2],"candidate1\n")==0))
		  	count[0]=count[0]+1;
		else if((strcmp(row[2],"candidate2\n")==0))
                  	count[1]=count[1]+1;
		else if((strcmp(row[2],"candidate3\n")==0))
                  	count[2]=count[2]+1;
		else if((strcmp(row[2],"candidate4\n")==0))
                  	count[3]=count[3]+1;
		else if((strcmp(row[2],"candidate5\n")==0))
                  	count[4]=count[4]+1;
		else if((strcmp(row[2],"candidate6\n")==0))
                  	count[5]=count[5]+1;
		else if((strcmp(row[2],"candidate7\n")==0))
                  	count[6]=count[6]+1;
		else if((strcmp(row[2],"candidate8\n")==0))
                  	count[7]=count[7]+1;
	
  	}
	printf("candidate 1 got %d votes\n",count[0]);
	printf("candidate 2 got %d votes\n",count[1]);
	printf("candidate 3 got %d votes\n",count[2]);
	printf("candidate 4 got %d votes\n",count[3]);
	printf("candidate 5 got %d votes\n",count[4]);
	printf("candidate 6 got %d votes\n",count[5]);
	printf("candidate 7 got %d votes\n",count[6]);
	printf("candidate 8 got %d votes\n",count[7]);

	int max=0;
	for(int i=0;i<8;i++)
	{
		if(count[i]>count[max])
		{
			max=i;
		}
	}
	
	for(int i=0;i<8;i++)
	if(count[i]==count[max])
	{
		printf("candidate%d:%d votes\n",i+1,count[max]);
	}

  	mysql_free_result(result);

	mysql_close(con);
	close(newsockfd);
	close(sockfd);
	return 0;
}
void decrypt()
{
  	long int pt, ct, key = d[0], k;
  	i = 0;
	for(i=0;i<strlen(en);i++)
  	{
    		ct = temp[i];
    		k = 1;
    		for(j = 0; j < key; j++)
    		{
      			k = k * ct;
      			k = k % 391;
    		}
   		pt = k + 96;
   		m[i] = pt;
  	}
}
