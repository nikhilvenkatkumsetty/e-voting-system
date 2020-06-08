#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<math.h>
//function execute when socket code does not work properly

long int p,q,multiply,t,flag,e[100],d[100],temp[100],j,i;
char en[100];
char m[100];
char msg[100];
int prime(long int);
void ce();
long int cd(long int);
void encrypt();


void error(const char * msg)
{
	perror(msg);
	exit(1);
}
int main(int argc , char * argv[])
{
	if(argc<3)
	{
		fprintf(stderr,"hostname or portnumber not entered\n");
		exit(1);
	}
	int sockfd , portno, n;
	char buffer[255];
	struct hostent * server;

	struct sockaddr_in serv_addr;
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
		error("error in creating socket\n");
	server = gethostbyname(argv[1]);
	if(server==NULL)
		fprintf(stderr,"error no such server\n");
	bzero((char*)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*)server->h_addr , (char*) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
		error("connection failed\n");

	printf("Welcome to voting database\n");
	printf("enter the userid\n");
	bzero(buffer,255);
	fgets(buffer,255,stdin);
	n = write(sockfd,buffer,strlen(buffer));
	if(n<0)
		error("error in writing\n");
	bzero(buffer,255);
	n=read(sockfd,buffer,255);
	if(n<0)
		error("error in reading\n");
	printf("%s\n",buffer);

	printf("enter the aadhar number\n");
	bzero(buffer,255);
	fgets(buffer,255,stdin);
	n=write(sockfd,buffer,strlen(buffer));
	if(n<0)
		error("error in writing\n");
	bzero(buffer,255);
	n=read(sockfd,buffer,255);
	if(n<0)
		error("error in reading\n");
//	printf("%s\n",buffer);
	int q=atoi(buffer);
	//printf("%d",atoi(buffer));
	if(q)
	{
		printf("you have already casted the vote\n");
		close(sockfd);
		return 0;
	}

	printf("enter the vote\n The candidates are\n 1: candidate1\n 2: candidate2 \n 3: candidate3\n 4: candidate4\n 5: candidate5\n 6: candidate6\n 7: candidate7\n8: candidate8\n");
	bzero(buffer,255);
        fgets(msg,255,stdin);

	p=23,q=17;
	strcpy(m,msg);
	multiply=p*q;
	t=(p-1)*(q-1);
	ce();
	encrypt();
	strcpy(buffer,en);


        n=write(sockfd,buffer,strlen(buffer));
        if(n<0)
                error("error in writing\n");
        bzero(buffer,255);
        n=read(sockfd,buffer,255);
        if(n<0)
                error("error in reading\n");
        printf("%s\n",buffer);


	for(i=0;i<strlen(msg);i++)
	{
		bzero(buffer,255);
		char var[100];
		sprintf(var,"%ld",temp[i]);
		strcpy(buffer,var);
		n=write(sockfd,buffer,strlen(buffer));
		if(n<0)
			error("error in writing\n");
		n=read(sockfd,buffer,255);
		if(n<0)
			error("error in reading\n");
		bzero(buffer,255);
	}

	char var[100];
	sprintf(var,"%ld",d[0]);
	strcpy(buffer,var);
	n=write(sockfd,buffer,strlen(buffer));
        if(n<0)
                error("error in writing\n");
	bzero(buffer,255);
	close(sockfd);
	return 0;
}
int prime(long int pr)
	{
		int i;
		j=sqrt(pr);
		for (i=2;i<=j;i++)
		{
        		if(pr%i==0)
        		return 0;
		}
        	return 1;
	}
void ce()
{
        int k;
        k=0;
        for (i=2;i<t;i++)
        {
                if(t%i==0)
                continue;
                flag=prime(i);
                if(flag==1&&i!=p&&i!=q)
                {
                        e[k]=i;
                        flag=cd(e[k]);
                        if(flag>0)
                        {
                                d[k]=flag;
                                k++;
                        }
                        if(k==99)
                                break;
                }
        }
}
long int cd(long int x)
{
        long int k=1;
        while(1)
        {
                k=k+t;
                if(k%x==0)
                        return(k/x);
        }
}
void encrypt()
{
        long int pt,ct,key=e[0],k,len;
        i=0;
        len=strlen(msg);
        while(i!=len)
        {
                pt=m[i];
                pt=pt-96;
                k=1;
                for (j=0;j<key;j++)
                {
                        k=k*pt;
                        k=k%multiply;
                }
                temp[i]=k;
                ct=k+96;
                en[i]=ct;
                i++;
        }
}
