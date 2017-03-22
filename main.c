#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include"mycommands.h"
#include <sys/utsname.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>


int EXIT=0;

long long int no_of_commands,no_of_arguments;
char dspace[6]=" \r\a\n\t";
char *current_directory,*host_name,*user_name;
int jobs[100];
char njobs[100][100];
int np=0;
int flj[100];

//********************function to run normal commands******************************//
void run_normal_command(char **arguments)
{
	char and;and='&';
	int i=1,flag=0,check=0,status;
	pid_t pid = fork();
	while(arguments[i]!=NULL)
	{
		if((arguments[i][0])==and){
			flag=i;
			arguments[i]=NULL;
			break;
		}
		i++;
	}

	if(pid<0)
	{
		fprintf(stderr,"Error Occured\n");
		return ;
	}
	if(pid==0)
	{
		signal( SIGINT , SIG_DFL);
		check=execvp(arguments[0],arguments);
		if(check<0)
			fprintf(stderr,"ERROR:Command can't found\n");
	}
	else
	{
		if(flag==0)
			waitpid(pid,&status,0);
		else{
			printf("%d\n",pid);
			strcpy(njobs[np],arguments[0]);
			jobs[np]=pid;
			flj[np]=1;
			np++;
		}
	}
}

//******************************************** piping related functions ***************************************************


//******************************** function to Execute single element of a piped command ***************************
int execute_single_command(int in, int out, char *com)
{
	pid_t pid;
	char *argument[1000];int i;
	char * Split_temp = strtok(com," ");
	for(i = 0; i < 1000; i++) {
		argument[i] = Split_temp;
		Split_temp = strtok(NULL," ");
		if(argument[i] == NULL) break;
	}
	i=1;
	while(argument[i]!=NULL)
	{
		if(strcmp(argument[i],"<")==0)
		{
			char* file = argument[i+1];
			int fd = open(file,O_RDWR,0777);
			dup2(fd,in);
			close(fd);
			argument[i]=NULL;
		}
		else if(strcmp(argument[i],">")==0)
		{
			char* file = argument[i+1];
			int fd = open(file,O_RDWR | O_CREAT,0777);
			if(dup2(fd,out) == -1) printf("some error occurred\n");
			close(fd);
			argument[i]=NULL;
		}
		else if(strcmp(argument[i],">>")==0)
		{
			char* file = argument[i+1];
			int fd = open(file,O_RDWR | O_CREAT | O_APPEND,0777);
			if(dup2(fd,out) == -1) printf("some error occurred\n");
			close(fd);
			argument[i]=NULL;
		}
		i++;
	}
	pid = fork();
	if (pid == 0)
	{
		int j=0;
		while(j<i)
		{
			//if(redirect==1)
			//	dup2(in,0);
			//else if(redirect==2)
			//	dup2(out,0);
			j++;
	
		}

		if (in != 0)
		{
			dup2 (in, 0);
			close (in);
		}

		if (out != 1)
		{
			dup2 (out, 1);
			close (out);
		}

		return execvp (argument[0], argument);
	}

	return pid;
}

//********************************** function to handle piping ********************************************

int handle_piping(char * comd)
{
	int std_in, std_out,splitedcommand = 0,totalcommand;
	std_in = dup(0);
	std_out = dup(1);
	int i;
	int in, fd [2];
	char * split;
	char com[100][100];
	if(comd[strlen(comd)-1] == '\n')
	{
		comd[strlen(comd)-1] = '\0';
	}
	int temp=0;
	split = strtok(comd,"|");
	while(split)
	{
		strcpy(com[temp],split);
		split = strtok(NULL,"|");
		temp++;
	}
	in = 0;

	for (i = 0; i < temp-1; ++i)
	{
		totalcommand = i;
		while(splitedcommand<totalcommand)
		{
			//pipe (fd);
			//execute_single_command(in, fd [1], com[i]);
			splitedcommand++;
		}
		pipe (fd);
		execute_single_command(in, fd [1], com[i]);
		close (fd [1]);
		in = fd [0];
	}

	char *argument[1000];
	char * Split_temp = strtok(com[i]," ");
	for(i = 0; i < 1000; i++)
	{
		argument[i] = Split_temp;
		Split_temp = strtok(NULL," ");
		if(argument[i] == NULL) break;
		}
		i=1;
		while(argument[i]!=NULL)
		{
			if(strcmp(argument[i],"<")==0)
			{
				char* file = argument[i+1];
				int fd = open(file,O_RDWR,0777);
				dup2(fd,in);
				close(fd);
				argument[i]=NULL;
			}
			else if(strcmp(argument[i],">")==0)
			{
				char* file = argument[i+1];
				int fd = open(file,O_RDWR | O_CREAT,0777);
				if(dup2(fd,1) == -1) printf("some error occurred\n");
				close(fd);
				argument[i]=NULL;
			}
			else if(strcmp(argument[i],">>")==0)
			{
				char* file = argument[i+1];
				int fd = open(file,O_RDWR | O_CREAT | O_APPEND,0777);
				if(dup2(fd,1) == -1) printf("some error occurred\n");
				close(fd);
				argument[i]=NULL;
			}
			i++;
		}

	pid_t pid = fork();
	if (pid == -1)
	{
		char* error = strerror(errno);
		printf("fork: %s\n", error);
		dup2(std_in, 0);
		dup2(std_out,1);
		close(std_in);
		close(std_out);
		return 1;
	}

	else if (pid == 0) {
		if (in != 0)
			dup2 (in, 0);
		execvp(argument[0], argument);
		dup2(std_in, 0);
		dup2(std_out,1);
		close(std_in);
		close(std_out);
		char* error = strerror(errno);
		printf("shell: %s: %s\n", argument[0], error);
		return 0;
	}
	else {
		int childStatus;
		waitpid(pid, &childStatus, 0);
		dup2(std_in, 0);
		dup2(std_out,1);
		close(std_in);
		close(std_out);
		return 1;
	}
	dup2(std_in, 0);
	dup2(std_out,1);
	close(std_in);
	close(std_out);
}

//********************************************* To check piping *********************************

int checkPipe(char* com)
{
	int l=strlen(com);
	int i;
	for(i=0;i<l;i++)
	{
		if(com[i]=='|')
		{
			return 1;
		}
	}
	return 0;
}

//*****************************************piping functions ends ***************************************



// ****************************** function to check redirection is present or not *********************
void check_redirection(char **arguments)
{
	int i=0,chk=0;
	char * file;
	for(i=1;i<no_of_arguments;i++)
	{
		if(strcmp(arguments[i],">")==0)
		{
			file = arguments[i+1];
			int fd = open(file,O_RDWR | O_CREAT,0777);
			if(dup2(fd,1) == -1) printf("some error occurred\n");
			close(fd);
			arguments[i]=NULL;
		}
		else if(strcmp(arguments[i],">>")==0)
		{

			file = arguments[i+1];
			int fd = open(file,O_RDWR | O_CREAT | O_APPEND,0777);
			if(dup2(fd,1) == -1) printf("some error occurred\n");
			close(fd);
			arguments[i]=NULL;
		}
		else if(strcmp(arguments[i],"<")==0)
		{
			file = arguments[i+1];
			int fd = open(file,O_RDWR,0777);
			dup2(fd,0);
			close(fd);
			arguments[i]=NULL;
		}
	}
	return ;
}


//*************************** function checks command and call required function *****************************

void run_command(char **arguments)
{
	int stdinp = dup(0);
	int stdout = dup(1);
	check_redirection(arguments);
	char cd[10],pwd[10],echo[10],pinfo[10],exit[10];
	strcpy(cd,"cd");strcpy(pwd,"pwd");strcpy(echo,"echo"),strcpy(pinfo,"pinfo"),strcpy(exit,"exit");
	if(strcmp(arguments[0],cd)==0)	mycd(arguments[1],current_directory);
	else if(strcmp(arguments[0],"jobs")==0)	myjobs(njobs , jobs , flj , np);
	else if(strcmp(arguments[0],"kjob")==0)	mykjobs(jobs[arguments[1][0]-'0'],arguments[2][0]-'0');
	else if(strcmp(arguments[0], "killallbg")==0)	mykillallbg(jobs , flj , np);
	else if(strcmp(arguments[0],pwd)==0)	mypwd();
	else if(strcmp(arguments[0],echo)==0)	myecho(arguments);
	else if(strcmp(arguments[0],pinfo)==0)	mypinfo(arguments);
	else if(strcmp(arguments[0],exit)==0)	EXIT=1;
	else if(strcmp(arguments[0],"fg")==0)
	{
		if(flj[arguments[1][0]-'0']==1)
			myfg(jobs[arguments[1][0]-'0']);
		else
			printf("Error: No jobs exists\n" );
	}
	else	run_normal_command(arguments);
	dup2(stdinp,0);
	dup2(stdout,1);
	close(stdinp);
	close(stdout);
	return ;
}

//*********** function to get command ****************

char *get_input()
{
	char *line = NULL;
	ssize_t bufsize = 0;
	getline(	&line, &bufsize ,stdin);
	return line;
}

//************ function to split any command into arguments*************

char **split_command(char *command)
{
	long long size=100,pos=0;
	char **arguments=(malloc(size*sizeof(char*)));
	char *token;

	token = strtok(command,dspace);

	while(token)
	{
		arguments[pos]=token;pos++;
		if(pos>=size)
		{
			size += 100;
			arguments = realloc(arguments,size*sizeof(char*));
		}

		token=strtok(NULL,dspace);
	}
	arguments[pos]=NULL;
	no_of_arguments=pos;
	return arguments;
}


//**************************** function to break each command ***************************

char **split_input(char *input)
{
	no_of_commands=0;
	long long size=100,pos=0;
	char **commands=(malloc(size*sizeof(char*)));
	char *token;

	token = strtok(input,";");

	while(token)
	{
		commands[pos]=token;
		pos++;

		if(pos>=size)
		{
			size += 100;
			commands = realloc(commands,size*sizeof(char*));
		}

		token=strtok(NULL,";");
	}
	commands[pos]=NULL;
	no_of_commands=pos;
	return commands;
}

//******************** function to pirnt display command prompt ***********************************


void print_display()
{
	int flag=0,i=0;
	char *nd;
	nd = malloc(1000*sizeof(char)); getcwd(nd,1000);
	while(current_directory[i]!='\0')
	{
		if(nd[i]=='\0')
		{
			flag=1;
			break;
		}
		if(current_directory[i]!=nd[i])
			flag=1;
		i++;
	}

	if(flag==0)
		printf("%s@%s:~%s $ ",user_name,host_name,&nd[i]);
	else
		printf("%s@%s:%s $ ",user_name,host_name,nd);
}

//************* function give user and host name ***************
void give_display()
{
	host_name = malloc(1000*sizeof(char));
	user_name = getenv("USER");
	int t = gethostname(host_name, 1000);
}


//****************** main function ****************************
int main()
{
	int it;
	for(it=0;it<100;it++)
		flj[it]=0;
	give_display();
	current_directory = malloc(1000*sizeof(char));	getcwd(current_directory,1000);
	while(1)
	{
		int status;pid_t pid;
		while((pid = waitpid(-1, &status, WNOHANG)) > 0)
		{
			for(int jk=0;jk<100;jk++){
				if(jobs[jk]==pid){
					flj[jk]=0;
					printf("[proc %d exited with code %d]\n",pid, WEXITSTATUS(status));
				}
			}
		}
		signal( SIGINT , SIG_IGN);
		no_of_commands =0;
		print_display();
		char *input;
		char **commands;
		char **arguments;
		input=get_input(input);
		if(input==NULL)
			EXIT=1;
		commands=split_input(input);
		char *point;
		for(int num=0;num<no_of_commands;num++)
		{
			if(checkPipe(commands[num]))
			{
				handle_piping(commands[num]);
				continue;
			}
			arguments=split_command(commands[num]);
			if(no_of_arguments!=0)
				run_command(arguments);
			free(arguments);
			if(EXIT==1)
				break;
		}
		if(EXIT==1 || no_of_commands==0)
			break;
	}
	exit(0);
	return 0;
}
