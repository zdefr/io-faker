#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <malloc.h>
#include <unistd.h>
int main(int argc, char* argv[]){
	//创建守护进程
//	if(fork()){
//		return 0;
//	}
//	setsid();

	struct sockaddr_in maddr,yaddr;
	socklen_t ysize;
	pid_t id[10]={0};
	int count=0;	//空闲id号
	char iort[10]="10010"; 	//默认端口10086

	if(argc>1){
		strcpy(iort, argv[1]);
	}
	
	//网络信息
	maddr.sin_family=AF_INET;
	maddr.sin_port=htons(atoi(iort));
	inet_pton(AF_INET,"0.0.0.0",(void*)&maddr.sin_addr);
	ysize=sizeof(yaddr);	

	//s-b-l
	int sw = socket(AF_INET,SOCK_STREAM,0);
	if(sw<0){
		perror("socket");
		exit(1);
	}
	if(bind(sw,(void*)&maddr,sizeof(maddr))<0){
		perror("bind");
		exit(1);
	}
	listen(sw,10);

	//监听端口
	while(1){
		int aw = accept(sw,(void*)&yaddr,&ysize);
		if(aw<0){
			perror("accept");
			exit(1);
		}else{
			//回收子进程
			for(int i=0;i<10;i++){
				if(id[i]!=0){
					int situ = waitpid(id[i],NULL,WNOHANG);
					if(situ!=0){
						id[i]=0;
					}
				}else{
					count = i;
				}
			}
		
			id[count] = fork();
			
			//父进程继续监听
			if(id[count]==0){
				close(aw);
				//exit(1);
				continue;
			}

			//子进程处理请求
			if(dup2(aw,fileno(stdin))<0){
				perror("dup2:in");
				exit(1);
			}
			//if(dup2(aw,fileno(stdout))<0){
			//	perror("dup2:out");
			//	exit(1);
			//}
			//char argv_set[2][10];
			char rdir[50];
			if(getcwd(rdir,50)==NULL){
				perror("getcwd");
				exit(1);
			}
			strcat(rdir,"/fio/io");
			//strcpy(argv_set[1],"/home/zde/xhttpd");
			if(execv(rdir,argv)<0){
				perror("execv");
				exit(1);
			}

		}
	}

//	close(aw);
	close(sw);

	return 0;
}
