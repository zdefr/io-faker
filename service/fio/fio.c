#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SEQ 10000

void rehead(char* mdir,char* ydir, int count,char* cond){
	printf("OK %s %s\n",mdir,ydir);
	printf("%s %d\n",cond,count);
	printf("finish\n");
	fflush(stdout);
}

void refile(char* mdir,char* ydir,int count,char* cond){
	int ow = open(mdir,O_RDONLY);
	off_t offset=count*SEQ;
	if(lseek(ow,offset,SEEK_SET)<0){
		perror("lseek");
		exit(1);
	}
	if(ow<0){
		perror("open");
		exit(1);
	}
	//rehead(mdir,ydir,(count+1),cond);
	char buf[100];
	int num;
	int countb=0;
	do{
		num=read(ow,buf,100);
		if(countb==0){
			rehead(mdir,ydir,(count+1),cond);
		}
		write(fileno(stdout),buf,num);
		countb++;
	}while(num==100&&countb<100);
	fflush(stdout);
	close(ow);

}

int main(){
	struct stat st_buf;
	char mid[100];
	char met[10],mdir[40],ydir[50];
	char tags[10],count[20];
	int tag,num;
	char cond[10];
	fgets(mid,100,stdin);
	sscanf(mid,"%s %s %s",met,mdir,ydir);
	

	fgets(mid,100,stdin);
	sscanf(mid,"%s %s",tags,count);
	
	while(strcmp(mid,"finish\n")!=0){
		fgets(mid,100,stdin);
	}


	if(strcmp(met,"GET")!=0){
		printf("methord wrong\n");
		exit(1);
	}

	if(strcmp(tags,"START")==0){
		tag=1;
		strcpy(cond,"CREAT");
	}else if(strcmp(tags,"CONTINUE")==0){
		tag=0;
		strcpy(cond,"FOUND");
	}else{
		tag=-1;
		strcpy(cond,"FINISH");
	}

	num=atoi(count);

	if(stat(mdir,&st_buf)<0){
		perror("stat");
		exit(1);
	}
	if(tag!=-1&&st_buf.st_size<=((num+1)*10000)){
		strcpy(cond,"FFOUND");
	}

	refile(mdir,ydir,num,cond);
	return 0; 
}
