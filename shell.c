#include<stdio.h>
#include<sys/utsname.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>



char input[1000],curdir[1000],homedir[1000],command[1000],temp[1000],temp1[1000],temp_pip[1000],up[1000];
int cnt=0,flag=0,countmain=0,countex_fore=0,countex_back=0,pip_ar[2],back=-1;

int pipe_cnt=0,t=0;  //for last pipe part. 

char** arg1;
char pids[100][4][100];



char** tokenize(char dl[],int* count)
{
	char** commands=(char**) malloc(sizeof(char*)*100);
	char* token;
	int j=-1;

	for(int i=0;i<100;i++)
	{
		commands[i] = (char*)malloc(sizeof(char)*100); 
	}


	token=(char*)malloc(sizeof(char)*100);
   
    token = strtok(input, dl);
   
    while( token != NULL ) 
    {
        strcpy(commands[++j],token); 
        token = strtok(NULL, dl);
    }
    *count=j;
    return commands;

}

char** tokenize1(char dl[],char t[],int* count)
{
	char** commands=(char**) malloc(sizeof(char*)*100);
	char* token;
	int j=-1;

	for(int i=0;i<100;i++)
	{
		commands[i] = (char*)malloc(sizeof(char)*100); 
	}


	token=(char*)malloc(sizeof(char)*100);
   
    token = strtok(t, dl);
   
    while( token != NULL ) 
    {
        strcpy(commands[++j],token);
        token = strtok(NULL, dl);
    }
    *count=j;
    return commands;

}


void handler()
{
	int status,fl=0;
	pid_t p=waitpid(-1,&status,WNOHANG);
	char *name=(char *)malloc(sizeof(char)*100);
	char *buf=(char *)malloc(sizeof(char)*100);
	sprintf(buf,"%d",p);
	for(int i=0;i<cnt;i++)
	{
      if(strcmp(pids[i][0],buf)==0)
      {
      	fl=1;
      	strcpy(name,pids[i][1]);

      	strcpy(pids[i][0],"-1");
      	break;
   
      }
	}
	if(fl==1)
	{
	 fprintf(stderr,"%s with pid %d exited normally\n",name,p);	
	 fflush(stdout);	
	}
}


void display()
{
	struct utsname uts;
	getcwd(curdir,1000);

	int flag=0;

	if(strlen(homedir)<=strlen(curdir))
	{
		
		for(int i=0;i<strlen(homedir);i++)
		{
			if(homedir[i]!=curdir[i])
			{
				flag=1;
				break;
			}
		}
		if(flag==0 && strlen(homedir)!=strlen(curdir))
		{
		    temp[0]='~';
		    int k=1;
		    
		    for(int i=strlen(homedir);i<=strlen(curdir);i++)
		    {
			    temp[k]=curdir[i];
			    k++;

		    }
		    temp[k]='\0';
		    for (int i=0;i<1000;i++)
		    {
			    curdir[i]=0;
		    }
		    for (int i=0;i<strlen(temp);i++)
		    {
			    curdir[i]=temp[i];
		    }
		}    
	
	    else if(strlen(homedir)==strlen(curdir)&& flag==0)
	    {
		    curdir[0]='~';
		    curdir[1]='\0';
	    }

    }
	if(uname(&uts)>=0)
	{
		sprintf(up,"%c %s %c %s %c %s %c ",'<',getenv("USER"),'@',uts.sysname,':',curdir,'>'); //uts.nodename
			
	}

	else
	{
		perror("Display Error");

	}

}


void command_finder()
{
	int inputlen=strlen(input);

	int i=0,k=0;
    while(i<inputlen){
    	if(input[i]!=' '){

    		break;
    	}
    	i++;
    }
	while(i<inputlen && input[i]!=' ')
		{
		command[k]=input[i];
		k++;
		i++;
	}
}

void arguments_finder()
{

    
    int commandlen =strlen(command),inputlen=strlen(input);
    int i=commandlen,i1=0;

    while(i1<inputlen)
    {
    	if(input[i1]!=' ')
    	{

    		break;
    	}

    	i1++;

    }
    i+=i1;
    while(i<inputlen && input[i]==' ' )
    {
    	i++;
    }
    
    for(int k=0;k<(inputlen-commandlen-i1);k++)
    {

    	temp[k]=input[i];
    	i++;
    }
    int index, i2;

    index = -1;

    i2 = 0;
    while(temp[i2] != '\0')
    {
        if(temp[i2] != ' ' && temp[i2] != '\t' && temp[i2] != '\n')
        {
            index= i2;
        }

        i2++;
    }
    temp[index + 1] = '\0';

    for (int i=0;i<1000;i++)
		{
			input[i]=0;
		}
    for (int i=0;i<strlen(temp);i++)
		{
			input[i]=temp[i];
		}

}

void echo()
{
	printf("%s\n",input);

}


void pwd()
{
	getcwd(curdir,1000);

	printf("%s\n",curdir);
}


void cd()
{
	if(input[0]=='~')
	{
		int k=1;
		strcpy(temp,homedir);
		for(int i=strlen(homedir);i<strlen(homedir)+strlen(input);i++)
		{
			temp[i]=input[k];
			k++;
		}
		int x=chdir(temp);
        if(x!=0)
        {
	        perror("Directory doesnot exist");
        };
		return;
	}

    int x=chdir(input);
    if(x!=0)
    {
	    perror("Directory doesnot exist");
    }

}

void ls()
{
	int flag1,flag2,flag3,flag4;
	flag1=flag2=flag3=flag4=0;
	char a[5],l[5],al[5],la[5];
	a[0]='-';
	a[1]='a';
	a[2]='\0';
	al[0]='-';
	al[1]='a';
	al[2]='l';
	al[3]='\0';
	l[0]='-';
	l[1]='l';
	l[2]='\0';
	la[0]='-';
	la[1]='l';
	la[2]='a';	
	la[3]='\0';

	int countls=0;


	char** commands=tokenize(" ",&countls);


	int check[countls+5];
	for(int i=0;i<countls+5;i++)
	{
		check[i]=0;
	}

	for(int i=0;i<=countls;i++)
	{
		if(strcmp(commands[i],a)==0)
		{
			flag4=1;
			check[i]=1;

		}
		if(strcmp(commands[i],l)==0)
		{
			flag3=1;
			check[i]=1;
		}
	    if( (strcmp(commands[i],al)==0) || (strcmp(commands[i],la))==0 )
		{
			flag2=1;
			check[i]=1;
		}
	}


	if(flag3==1&&flag4==1)
	{
		flag2=1;
	}
	if(flag2==0&&flag3==0&&flag4==0)
	{
		flag1=1;
	}


	int num=0;
	for(int i=0;i<=countls;i++)
	{
		if(check[i]==0)
		{
		    num=i+1;

	    }
	}



	if(num==0)
	{
		getcwd(temp,1000);
    	strcpy(input,temp);
	}

    else
    {

	    strcpy(input,commands[num-1]);

    }


	struct dirent *di;
	struct stat st;
	DIR *dir;
	dir=opendir(input);
	
	if(dir==NULL)
	{
		perror("Directory Error");
		return ;
	}



	if(flag2==1)
	{
		di=readdir(dir);

		while(di!=NULL)
		{
			
			
				strcpy(temp,input);
				strcat(temp,"/");
                strcat(temp,di->d_name);
				stat(temp,&st);
		        printf((S_ISDIR(st.st_mode))?"d":"-");
		        printf((st.st_mode & S_IRUSR)? "r":"-");
                printf((st.st_mode & S_IWUSR)? "w":"-");
                printf((st.st_mode & S_IXUSR)? "x":"-");
                printf((st.st_mode & S_IRGRP)? "r":"-");          /////changeeeee
                printf((st.st_mode & S_IWGRP)? "w":"-");
                printf((st.st_mode & S_IXGRP)? "x":"-");
                printf((st.st_mode & S_IROTH)? "r":"-");
                printf((st.st_mode & S_IWOTH)? "w":"-");
                printf((st.st_mode & S_IXOTH)? "x":"-");
                printf("  ");

                
                if(S_ISDIR(st.st_mode)==0)
                {
                	printf("%4d  ",1 );
                	
                }
                else
                {
                	int num_directories=0;

                	struct dirent *di1;
                	struct stat st1;
                	DIR *dir1;
                	dir1=opendir(temp);
                	di1=readdir(dir1);
                	while(di1 !=NULL)
                	{
                		strcpy(temp1,temp);
                		strcat(temp1,"/");
                 	    strcat(temp1,di1->d_name);
                	    stat(temp1,&st1);


                		if(S_ISDIR(st1.st_mode)!=0)
                        {
                        	num_directories++;
                	
                        }
                        di1=readdir(dir1);

                	}

                	printf("%4d  ",num_directories );

                }



                struct passwd *uname;
                struct group *gname;
                uname=getpwuid(st.st_uid);
                gname=getgrgid(st.st_gid);
                printf("%s  ",uname->pw_name);
                printf("%s  ",gname->gr_name );

                printf("%6ld  ",st.st_size);

                char *ct=(char*)malloc(sizeof(char)*100);
                ct=ctime(&st.st_mtime);
                for(int k=4;k<strlen(ct)-9;k++)
                printf("%c",ct[k]);
                printf(" ");


		        printf("%s",di->d_name );
		        printf("\n");

		    
		    di=readdir(dir);

        }



	}	
	else if(flag3==1)
	{
		di=readdir(dir);

		while(di!=NULL)
		{
			if(di->d_name[0]!='.')
			{
				strcpy(temp,input);
				strcat(temp,"/");
                strcat(temp,di->d_name);
				stat(temp,&st);
		        printf((S_ISDIR(st.st_mode))?"d":"-");
		        printf((st.st_mode & S_IRUSR)? "r":"-");
                printf((st.st_mode & S_IWUSR)? "w":"-");
                printf((st.st_mode & S_IXUSR)? "x":"-");
                printf((st.st_mode & S_IRGRP)? "r":"-");          /////changeeeee
                printf((st.st_mode & S_IWGRP)? "w":"-");
                printf((st.st_mode & S_IXGRP)? "x":"-");
                printf((st.st_mode & S_IROTH)? "r":"-");
                printf((st.st_mode & S_IWOTH)? "w":"-");
                printf((st.st_mode & S_IXOTH)? "x":"-");
                printf("  ");

                
                if(S_ISDIR(st.st_mode)==0)
                {
                	printf("%4d  ",1 );
                	
                }
                else
                {
                	int num_directories=0;

                	struct dirent *di1;
                	struct stat st1;
                	DIR *dir1;
                	dir1=opendir(temp);
                	di1=readdir(dir1);
                	while(di1 !=NULL)
                	{
                		strcpy(temp1,temp);
                		strcat(temp1,"/");
                 	    strcat(temp1,di1->d_name);
                	    stat(temp1,&st1);


                		if(S_ISDIR(st1.st_mode)!=0)
                        {
                        	num_directories++;
                	
                        }
                        di1=readdir(dir1);

                	}

                	printf("%4d  ",num_directories );

                }



                struct passwd *uname;
                struct group *gname;
                uname=getpwuid(st.st_uid);
                gname=getgrgid(st.st_gid);
                printf("%s  ",uname->pw_name);
                printf("%s  ",gname->gr_name );

                printf("%6ld  ",st.st_size);

                char *ct=(char*)malloc(sizeof(char)*100);
                ct=ctime(&st.st_mtime);
                for(int k=4;k<strlen(ct)-9;k++)
                printf("%c",ct[k]);
                printf(" ");


		        printf("%s",di->d_name );
		        printf("\n");

		    }
		    di=readdir(dir);

        }

	}
	else if (flag4==1)
	{

		di=readdir(dir);
		while(di!=NULL)
		{
			printf("%s   ",di->d_name );
			di=readdir(dir);

		}
		printf("\n");


	}
	else if (flag1==1)
	{

		di=readdir(dir);
		while(di!=NULL)
		{	
			if(di->d_name[0]!='.')
			printf("%s   ",di->d_name );
			di=readdir(dir);

		}
		printf("\n");

	}

return ;

}

void ls_back()
{
	pid_t pid=fork();
	if(pid==0)
	{
		ls();

	}

}


void pinfo()
{
	int pid;

	if(strcmp(input,"")==0)
	{
		// printf("%s %d\n","pid --",getpid() );
		pid=getpid();
		sprintf(input,"%d",pid);
    } 

	char buf[100]="/proc/",buf1[100]="/proc/";
	strcat(buf,input);
	strcat(buf1,input);

	strcat(buf,"/status");            ////buf=>status,buf1=>cmdline
	strcat(buf1,"/cmdline");

	FILE *fi1=fopen(buf,"r");
	if(fi1==NULL)
	{
		printf("NON-EXISTENT PID\n");
		return ;
	}
	FILE *fi2=fopen(buf,"r");
	if(fi2==NULL)
	{
		printf("NON-EXISTENT PID\n");
		return ;
	}
	printf("%s %s\n","pid --",input );
	char  *state,*vmsize;


	while(fgets(temp,50,fi1)!=NULL)
	{
		state=strstr(temp,"State");
		if(state!=NULL)
		{
		
		int k=0;

		while(state[k]!=':')
		{
			k++;
		}


        printf("Process Status -- ");
		for(int i=k+2;i<strlen(temp);i++)
		{
			printf("%c",state[i]);
		}

	    }

	}
				fclose(fi1);



	while(fgets(temp,50,fi2)!=NULL)
	{
		vmsize=strstr(temp,"VmSize");
		if(vmsize!=NULL)
		{
		
		int k=0;

		while(vmsize[k]!=':')
		{
			k++;
		}


        printf("memory ​ --");
		for(int i=k+2;i<strlen(temp);i++)
		{
			printf("%c",vmsize[i]);
		}

	    }

	}
				fclose(fi2);

	FILE *fi3=fopen(buf1,"r");
	char c;
	if(fi3!=0)
			{
				printf("Executable Path  --  ");
				while((c = getc(fi3)) != EOF)
				{
					putchar(c);
				}

				fclose(fi3);
			}
			printf("\n");

}


int get_status(int pid,char *c)
{
		char *cp_temp1 = temp1;
		int i=0;



		char *temp1=(char *)malloc(sizeof(char)*100);
		char *temp=(char *)malloc(sizeof(char)*100000);
		strcpy(temp,"/proc/");
		sprintf(temp1,"%d",pid);
		strcat(temp1,"/status/");
		strcat(temp,temp1);
		int f=open(temp,O_RDONLY);

		strcpy(temp1,"State");

		
		if(f==-1)		
		{
			free(temp);
			free(temp1);
			return -1;
		}
		read(f,temp,100000);
		
		temp1=strstr(temp,temp1);
		
		while(1)
		{
			if(temp1[i]=='\n')
			{
				temp1[i]=0;
				break;
			}
			i++;
		}
		
		*c=*(temp1+7);
		if(temp!=NULL) 
		{
				free(temp);
		}
		if(cp_temp1!=NULL)
		{	
			free(cp_temp1);
		}
		close(f);
}


void execv_fore()
{
	char **arg=(char**)malloc(sizeof(char*)*1000);
	for(int i=0;i<100;i++)
	{
		arg[i] = (char*)malloc(sizeof(char)*1000); 
	}
	

	strcpy(temp,command);
	strcat(temp," ");
	strcat(temp,input);
    
    countex_fore=0; 
	arg=tokenize1(" ",temp,&countex_fore);

	char buf[100]="/usr/bin/",buf1[100]="/bin/",buf2[100]="/usr/sbin",buf3[100]="/sbin/",buf4[100]="";
	strcat(buf,command);
	strcat(buf1,command);
	strcat(buf2,command);
	strcat(buf3,command);
	strcat(buf4,command);


	pid_t pid=fork();
	if(pid==-1)
	{
		perror("Can't fork()");

	}
	else if(pid==0)
	{
	 arg[countex_fore+1]=NULL;
		
		if((execv(buf,arg)<0)&&(execv(buf1,arg))&&(execv(buf2,arg))&&(execv(buf3,arg))&&(execv(buf4,arg)))
		{
			printf("%s doesn't exist\n",command);
		    exit(0);
	    }

	}
	else
	{
		wait(NULL);
	}
	for(int i=0;i<=countex_fore+1;i++)
	{
		free(arg[i]);
	}

}

void execv_back()
{
	char **arg=(char**)malloc(sizeof(char*)*1000);
	for(int i=0;i<100;i++)
	{
		arg[i] = (char*)malloc(sizeof(char)*1000); 
	}
	countex_back=0;


	arg=tokenize(" ",&countex_back);

	char buf[100]="/usr/bin/",buf1[100]="/bin/",buf2[100]="/usr/sbin",buf3[100]="/sbin/",buf4[100]="";
	strcat(buf,command);
	strcat(buf1,command);
	strcat(buf2,command);
	strcat(buf3,command);
	strcat(buf4,command);
	strcat(buf4,command);



	pid_t pid=fork();
	int status;
	
	if(pid==-1)
	{
		perror("Can't fork()");

	}
	else if(pid==0)
	{
	 arg[countex_back+1]=NULL;
		
		if((execv(buf,arg)<0)&&(execv(buf1,arg))&&(execv(buf2,arg))&&(execv(buf3,arg))&&(execv(buf4,arg)))
			perror(command);

	}

    signal(SIGCHLD,handler);
    sprintf(pids[cnt][0],"%d",pid);
    strcpy(pids[cnt][1],command);
    cnt++;
	
	for(int i=0;i<=countex_back+1;i++)
	{
		free(arg[i]);
	}
	

}


int execute(void)
{
	int flag_cd = 0,count_env,num1=0;
	flag=0;

	/*to find no.of arg for env*/
	char* temp_env=(char*)malloc(sizeof(char)*100);
	strcpy(temp_env,input);
	char **arg_env=(char**)malloc(sizeof(char*)*1000);
	for(int i=0;i<100;i++)
	{
		arg_env[i] = (char*)malloc(sizeof(char)*1000); 
	}
	arg_env=tokenize(" ",&count_env);

	strcpy(input,temp_env);	

    

    for(int x=0;x<strlen(input);x++)                  ///////////// To check whether the process is Background or Not
	{
		if(input[x]=='&')
		{
			input[x]='\0';
		    flag=1;
		 	break;
		}
		
	}

	if(strcmp(command,"cd")==0)
	{
		flag_cd=1;
		cd();
	}
	else if (strcmp(command,"unsetenv")==0)
	{
		flag_cd=1;
		if(count_env==0)
		{
			unsetenv(arg_env[0]);
		}
		else
		{
			fprintf(stderr,"Invalid number of arguments\n");
		}


	}
	else if(strcmp(command,"setenv")==0)
	{
		flag_cd=1;
		if( (count_env<0) || (count_env > 1))
		{
			fprintf(stderr,"Invalid number of arguments\n");
		}
		else
		{
			strcpy(temp_env,arg_env[0]);
			strcat(temp_env,"=");
			if(count_env==1)
				strcat(temp_env,arg_env[1]);
			putenv(temp_env);
		}

	}
	else if(strcmp(command,"getenv")==0)
	{
		flag_cd=1;
		if(count_env==0&&getenv(arg_env[0])!=0)
			printf("%s\n",getenv(arg_env[0]));
		else
			printf("%s\n","");

	}
	else if(strcmp(command,"jobs")==0)
	{
		int job_num=0;
		flag_cd=1;

		for(int num=0;num<cnt;num++)
		{
			if(strcmp(pids[num][0],"-1")!=0)
			{
				if(strcmp(pids[num][2],"r")==0)
					printf("[%d]  Running %s  [%s]\n",++job_num,pids[num][1],pids[num][0]);
				else
					printf("[%d]  Stopped %s  [%s]\n",++job_num,pids[num][1],pids[num][0]);

			}

		}

	}
	else if(strcmp(command,"kjob")==0)
	{
		int job_num=atoi(arg_env[0]);
		flag_cd=1;
		if(count_env!=1)
			fprintf(stderr,"Invalid number of arguments\n");
		else
		{
			for(num1=0;num1<=cnt;num1++)
			{
				if(strcmp(pids[num1][0],"-1")!=0)
				{
					job_num--;
				}
				if(job_num==0)
				{
					// printf("%d  %s \n",atoi(pids[num][0]),pids[num][1]);
					kill(atoi(pids[num1][0]),atoi(arg_env[1]));
					// strcpy(pids[num1][0],"-1");
 					
					break;
					
				}

			}
			sleep(1);
			char c;
			if(num1>cnt)
				num1=cnt;
			
			int flag_s=get_status(atoi(pids[num1][0]),&c);       ///send killed pid
			// printf("%c\n",c );

			if(flag_s==-1 || c=='Z')
			{
				strcpy(pids[num1][0],"-1");
			}
			else if(c=='R' || c=='S' )
			{
				strcpy(pids[num1][2],"r");

			}
			else if(c=='T')
			{
				strcpy(pids[num1][2],"s");
			}

		}
	}
	else if(strcmp(command,"bg")==0)
	{
		int job_num=atoi(arg_env[0]);
		flag_cd=1;
		if(count_env!=0)
			fprintf(stderr,"Invalid number of arguments\n");
		else
		{
			for(int num=0;num<=cnt;num++)
			{
				if(strcmp(pids[num][0],"-1")!=0)
				{
					job_num--;
				}
				if(job_num==0)
				{
					kill(atoi(pids[num][0]),18);

					 strcpy(pids[num][2],"r");
 					
					break;
					
				}

			}
		}


	}
	// else if(strcmp(command,"fg")==0)
	// {
	// 	char stdin_path[100];


	// 	int job_num=atoi(arg_env[0]);
	// 	int pid_fg=0,tmp_in=dup(0),new_stdin=0,sus_flag=0,num=0;
	// 	flag_cd=1;
	// 	if(count_env!=0)
	// 		fprintf(stderr,"Invalid number of arguments\n");
	// 	else
	// 	{
	// 		for(num=0;num<=cnt;num++)
	// 		{
	// 			if(strcmp(pids[num][0],"-1")!=0)
	// 			{
	// 				job_num--;
	// 			}
	// 			if(job_num==0)
	// 			{
	// 				pid_fg=atoi(pids[num][0]);
				
 					
	// 				break;
					
	// 			}

	// 		}
	// 	}
	// 	kill(pid_fg,SIGCONT);
	// 	sprintf(stdin_path,"/proc/%d/fd/0",pid_fg);
	// 	 new_stdin=open(stdin_path,O_APPEND);
	// 	dup2(new_stdin,0);
	// 	 // sus_flag=0;
	// 	back=pid_fg;
	// 	while(1)
	// 	{
	// 		char c1;
	// 		int yes=get_status(pid_fg,&c1);
	// 		if(yes==-1 || c1=='Z')
	// 			break;
	// 		else if(c1=='T')
	// 		{	
	// 			sus_flag=1;
	// 			break;
	// 		}
	// 	}
	// 	dup2(tmp_in,0);
	// 	close(tmp_in);
	// 	close(new_stdin);
	// 	back=-1;
	// 	if(sus_flag!=0)
	// 		strcpy(pids[num][2],"s");
	// 	else
	// 		strcpy(pids[num][0],"-1");
	// 	return 0;


	// }
	else if(strcmp(command,"overkill")==0)
	{
		flag_cd=1;
		for(int num=0;num<cnt;num++)
		{
			if(strcmp(pids[num][0],"-1")!=0)
			{
				kill(atoi(pids[num][0]),SIGKILL);
				strcpy(pids[num][0],"-1");
			}

		}


	}
	else if(strcmp(command,"quit")==0)
	{
		flag_cd=1;
		exit(0);
	}



if(flag_cd!=1)
{


	pid_t pid_pipe=fork();

	back=pid_pipe;

	if(pid_pipe<0)
		fprintf(stderr,"Can't fork()\n");
	else if(pid_pipe==0)
	{

		if(t!=pipe_cnt)
		{
			dup2(pip_ar[1],1);
		}
		
		if(strcmp(command,"echo")==0)
		{
			    echo();
		}
		else if(strcmp(command,"pwd")==0)
		{
			    pwd();
		}
		    
		else if(strcmp(command,"ls")==0)
		{
		    	ls();
		}
		
		else if(strcmp(command,"pinfo")==0)
		{
		 	pinfo();
		}
		    
		else
		{
		    

		    	char **arg=(char**)malloc(sizeof(char*)*1000);
				for(int i=0;i<100;i++)
				{
					arg[i] = (char*)malloc(sizeof(char)*1000); 
				}
	

				strcpy(temp,command);
				strcat(temp," ");
				strcat(temp,input);

			    
			    countex_fore=0; 
				arg=tokenize1(" ",temp,&countex_fore);

				char buf[100]="/usr/bin/",buf1[100]="/bin/",buf2[100]="/usr/sbin",buf3[100]="/sbin/",buf4[100]="";
				strcat(buf,command);
				strcat(buf1,command);
				strcat(buf2,command);
				strcat(buf3,command);
				strcat(buf4,command);

				 arg[countex_fore+1]=NULL;
					
					if((execv(buf,arg)<0)&&(execv(buf1,arg)<0)&&(execv(buf2,arg)<0)&&(execv(buf3,arg)<0)&&(execv(buf4,arg)<0))
					{
						fprintf(stderr, "%s doesn't exist\n",command);
					    exit(0);
				    }

				
				for(int i=0;i<=countex_back+1;i++)
				{
					free(arg[i]);
				}
		    		
		    

		}
		if(t!=pipe_cnt)
		   	close(pip_ar[1]);
		exit(0);


	}
	else 
	{
		
		if(flag==0)
		{
			waitpid(pid_pipe,NULL,WUNTRACED);
			back=-1;

		}
		else
		{
			back=-1;
			setpgid(pid_pipe,pid_pipe);
			
			signal(SIGCHLD,handler);
			sprintf(pids[cnt][0],"%d",pid_pipe);
			strcpy(pids[cnt][1],command);
			strcpy(pids[cnt++][2],"r");

		}
	}

}
	

}

void ctrlc()
{
	if(back!=-1)
	{
		kill(back,SIGKILL);
		
	}


	return;
}
void ctrlz()
{
	if(back!=-1)
	{
		kill(back,SIGTSTP);
		setpgid(back,back);

		sprintf(pids[cnt][0],"%d",back);
		strcpy(pids[cnt][1],command);
		strcpy(pids[cnt++][2],"s");

	}
}
char* in=NULL;

int main()
{
	getcwd(homedir,1000);
	signal(SIGINT,ctrlc);
	signal(SIGTSTP,ctrlz);

	using_history();

	while(1)
	{
        

		for (int i=0;i<1000;i++)
		{
			input[i]=0;
			temp[i]=0;
			command[i]=0;
			curdir[i]=0;
			temp1[i]=0;
		}
        
        display();

		in=readline(up);
		add_history(in);
		strcpy(input,in);

        countmain=0;          
        char** commands=tokenize(";",&countmain);
        int k=0;
		while(k<=countmain)
		{
			

			 flag=0;               ////to check &

		    for (int i=0;i<1000;i++)
		    {
			    input[i]=0;
			    temp[i]=0;
			    command[i]=0;
			    curdir[i]=0;
			    temp1[i]=0;
		    }
		    
		    strcpy(input,commands[k]);

		    int fd_in,fd_out;
		    fd_in=dup(0);
		    fd_out=dup(1);



			int flag_2=0;                                                       ////to check whether >> is there or not;

            if((input[0]=='>' ) ||( input[strlen(input)-1]=='>' ))
            	flag_2 = -1;

            if(flag_2 == -1){
            	k++;
            	continue;
            }

			for (int p=1;p<strlen(input)-1;p++)
			{
				if(input[p]=='>' && input[p+1]=='>')
					flag_2=1;
			}

			


		    int fd1=0,countmain2=0,countmain1=0,flag1=0,flag_1=0;

    
            char** re_out;         //// to split and store acc to > or >>;
 			
 			if(flag_2 == 1)
 			{
 				flag1=1;
 				re_out=tokenize(">>",&countmain1);

      			if(countmain1 > 1 ){

      				fprintf(stderr,"Not Correct Format \n");
      				k++;
      				continue;
      			}
      			else if(countmain1 == 1)
           		{

 				for(int p=0;p<strlen(re_out[0]);p++)
 				{
 					if(re_out[0][p]=='>')
 					{
 						fprintf(stderr,"Not correct Format \n");
 						k++;
 						continue;
 					}
 				}


 				for(int p=0;p<strlen(re_out[1]);p++)
 				{
 					if(re_out[1][p]=='>')
 					{
 						fprintf(stderr,"Not Correct Format \n");
 						k++;
 						continue;
 					}
 				}

 			    }

 		    }
 		    else if (flag_2 == 0)
 		    {
 		    	re_out=tokenize(">",&countmain1);
 		    	if(countmain1 == 1)
 		    	{
 		    		flag1=1;
 		    		flag_1=1;
 		    	}
 		    	else if(countmain1>1)
 		    	{
 		    		fprintf(stderr,"Not Correct Format \n");
 						k++;
 						continue;

 		    	}


 		    }

			char** re_out1=tokenize1(" ",re_out[1],&countmain2);

			if(flag_1 ==1)
			{
				strcpy(re_out[1],re_out1[0]);
			    strcpy(input,re_out[0]);
				fd1 = open(re_out[1],O_WRONLY | O_CREAT| O_TRUNC ,0644);

			}
			else if(flag_2 ==1)
			{
				    strcpy(re_out[1],re_out1[0]);
				    strcpy(input,re_out[0]);
				    fd1 = open(re_out[1],O_WRONLY | O_CREAT| O_APPEND ,0644);
			}



			/*********pipe Implementation*////////////
			write(0,NULL,0);

			pipe_cnt=0;

			char** pip=tokenize("|",&pipe_cnt);

			int flag_inre=0;


			for(t=0;t<pipe_cnt;t++)
			{
				strcpy(input,pip[t]);
				
				char* in_red;
				char** in_re;
				int count_in=0,fd_inred=0;


				in_red=strstr(input,"<");

				if(in_red!=0)
				{
					if(t!=0)
					{
						fprintf(stderr, "Not Correct Format\n");
					}
					else
					{
						flag_inre=1;
						in_re=tokenize("<",&count_in);
						if(count_in>1)
							fprintf(stderr, "Not correct format\n");
						else if(count_in==1)
						{
							count_in=0;
							char** in_re1=tokenize1(" ",in_re[1],&count_in);
							if(count_in==0)
							{
                                fd_inred=open(in_re1[0],O_RDONLY ,0644);

                                if(fd_inred==-1)
                                {
                                	fprintf(stderr, "File not found\n");
                                	k++;
                                	continue;
                                }
                                dup2(fd_inred,0);

							}

							strcpy(input,in_re[0]);

						}

					}
				}


				

				pipe(pip_ar);


		    	command_finder();
		    
		    	if(strlen(command)==0)
		    	{
		    		k++;
		    		
		    		continue;
		    	}
		    	arguments_finder();


		    	execute();

		    	close(pip_ar[1]);
		    	
				dup2(pip_ar[0],0);

		}

		if(t==pipe_cnt)
		{

		   		if(flag1==1 )              // To redirect to File if redirection exists,in last pipe part.
		   		{
		   	 		dup2(fd1,1);
		   		}

		   		strcpy(input,pip[pipe_cnt]);

		   		char* in_red;
		   		char** in_re;
		   		int fd_inred=0,count_in=0;

		   		in_red=strstr(input,"<");

				if(in_red!=0)
				{
					if(t!=0)
					{
						fprintf(stderr, "Not Correct Format\n");
						
					}
					else
					{
						flag_inre=1;
						in_re=tokenize("<",&count_in);

						if(count_in>1)
							fprintf(stderr, "Not correct format\n");
						else if(count_in==1)
						{
							count_in=0;

							char** in_re1=tokenize1(" ",in_re[1],&count_in);
							if(count_in==0)
							{
                                fd_inred=open(in_re1[0],O_RDONLY | O_CREAT ,0644);
                                if(fd_inred==-1)
                                {
                                	fprintf(stderr, "File not found\n");
                                	k++;
                                	continue;
                                }

                                dup2(fd_inred,0);
							}

						}


					}
				}

		    	command_finder();
		    
		    	if(strlen(command)==0)
		    	{
		    		k++;
		    		if(flag1=1)
		    		{
		    			dup2(fd_out,1);
		    	    	dup2(fd_in,0);
		    		}
		    		continue;
		    	}
		    	arguments_finder();

		    	execute();
		}

		dup2(fd_out,1);
		dup2(fd_in,0);
	    k++;
	    }
	    for(int s=0;s<=k;s++)
	    free(commands[s]); 

	}
	return 0; 
}
