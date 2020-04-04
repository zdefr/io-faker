#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
int main(int argc,char* argv[]){
	char met[10],mdir[50],ydir[50],bdir[50];
	char mid[100];
	char *check;
	char tags[10],count[20];
	off_t offset=0;
	strcpy(bdir,"/buffer/buffer");
	strcat(bdir,argv[1]);

	umask(0000);
	int ow = open(bdir,O_CREAT | O_WRONLY,0777);
	int numc;
	do{
		numc = read(fileno(stdin),mid,100);
		write(ow,mid,numc);
	}while(numc==100);
	close(ow);
	close(fileno(stdout));
	
	int rw = open(bdir,O_RDONLY);
	dup2(rw,fileno(stdin));
	fgets(mid,100,stdin);
	offset+=strlen(mid);
	sscanf(mid,"%s %s %s",met,ydir,mdir);
	
	fgets(mid,100,stdin);
	sscanf(mid,"%s %s",tags,count);
	offset+=strlen(mid);
	while(strcmp(mid,"finish\n")){
		fgets(mid,100,stdin);
		offset+=strlen(mid);
	}

	if(strcmp(tags,"FINISH")==0){
		close(rw);
		int bf = open(mdir,O_CREAT | O_WRONLY | O_APPEND,0777);
		int mf = open(bdir,O_RDONLY);
		lseek(mf,offset,SEEK_SET);
		do{
			numc = read(mf,mid,100);
			write(bf,mid,numc);
		}while(numc==100);
		close(bf);
		close(fileno(stdin));
		chmod(mdir,0777);
		remove(bdir);
		exit(1);
	}

	if(strcmp(tags,"FFOUND")==0){
		strcpy(tags,"FINISH");
	}

	if(strcmp(tags,"CREAT")==0){
		strcpy(tags,"CONTINUE");
	}

	if(strcmp(tags,"FOUND")==0){
		strcpy(tags,"CONTINUE");
	}
	close(rw);
	
	if(fork()==0){
		int bf = open(mdir,O_CREAT | O_WRONLY | O_APPEND);
		int mf = open(bdir,O_RDONLY);
		lseek(mf,offset,SEEK_SET);
		do{
			numc = read(mf,mid,100);
			write(bf,mid,numc);
		}while(numc==100);
		close(bf);
		close(fileno(stdin));
		remove(bdir);
		exit(1);
	}else{
		char rdir[50];
		if(getcwd(rdir,50)==NULL){
			perror("getcwd");
			exit(1);
		}
		strcat(rdir,"/iou");

		execl(rdir,"iou","GET",ydir,mdir,tags,count,NULL);
	}

/*	do{
		check=fgets(mid,100,stdin);
	}while(*check==EOF);
	sscanf(mid,"%s %s %s",met,ydir,mdir);
	
	fgets(mid,100,stdin);
	sscanf(mid,"%s %s",tags,count);
	
	while(strcmp(mid,"finish\n")){
		fgets(mid,100,stdin);
	}
	fflush(stdin);
	int ow = open(mdir,O_APPEND | O_CREAT | O_WRONLY);
	if(ow<0){
		perror("open");
		exit(1);
	}
	int numc;
	char did;
	do{
		did=getchar();
		//numc=read(fileno(stdin),mid,100);
		write(ow,&did,1);
	}while(did!=EOF);

	return 0;

	*/
}
