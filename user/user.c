#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>

int main(int argc,char* argv[]){
	char mdir[100],ydir[100],met[10];
	char count[20],tags[10];
	struct sockaddr_in yaddr;

	if(argc<4){
		scanf("%s %s %s",met,ydir,mdir);
		strcpy(count,"0");
		strcpy(tags,"START");
	}else{
		strcpy(met,argv[1]);
		strcpy(ydir,argv[2]);
		strcpy(mdir,argv[3]);
		strcpy(tags,argv[4]);
		strcpy(count,argv[5]);
	}

	int con = socket(AF_INET,SOCK_STREAM,0);
	if(con<0){
		perror("socket");
		exit(1);
	}

	yaddr.sin_family=AF_INET;
	yaddr.sin_port=htons(10010);
	inet_pton(AF_INET,"0.0.0.0",&yaddr.sin_addr);

	if(connect(con,(void*)&yaddr,sizeof(yaddr))<0){
		perror("connect");
		exit(1);
	}

	if(dup2(con,fileno(stdin))<0){
		perror("dup2:in");
		exit(1);
	}
	if(dup2(con,fileno(stdout))<0){
		perror("dup2:out");
		exit(1);
	}

	printf("%s %s %s\n",met,ydir,mdir);
	printf("%s %s\n",tags,count);
	printf("finish\n");
	fflush(stdout);
	
		char rdir[50];
		if(getcwd(rdir,50)==NULL){
			perror("getcwd");
			exit(1);
		}
		strcat(rdir,"/c");


	execl(rdir,"c",count,NULL);

}
