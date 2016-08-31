#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include"mycommands.h"


long long int no_of_commands;
char dspace[6]=" \r\a\n\t";
char *current_directory,*host_name,*user_name;
void give_display()
{
	host_name = malloc(1000*sizeof(char));
	user_name = getenv("USER");
	int t = gethostname(host_name, 1000);
}


void run_normal_command(char **arguments)
{
	char and;and='&';
	int i=1,check=0,flag=0,status;
	pid_t pid = fork();
	signal(SIGCHLD, SIG_IGN);
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
		printf("Error Occured\n");
		return ;
	}
	if(pid==0)
	{
		check=execvp(arguments[0],arguments);
		if(check<0)
			printf("ERROR:Command can't found\n");
	}
	else
	{
		if(flag==0)
			waitpid(pid,&status,0);
		else
			printf("%d\n",pid);
	}
}

void run_command(char **arguments)
{
	char cd[10],pwd[10],echo[10];
	strcpy(cd,"cd");strcpy(pwd,"pwd");strcpy(echo,"echo");
	if(strcmp(arguments[0],cd)==0)	mycd(arguments[1],current_directory);
	else if(strcmp(arguments[0],pwd)==0)	mypwd();
	else if(strcmp(arguments[0],echo)==0)	myecho(arguments);
	else	run_normal_command(arguments);
	return ;
}


char *get_input()
{
	char *line = NULL;
	ssize_t bufsize = 0; 
	getline(&line, &bufsize ,stdin);
        return line;
}


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
	return arguments;
}




char **split_input(char *input)
{
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
		printf("%s@%s:~/%s$ ",user_name,host_name,&nd[i]);
	else	
		printf("%s@%s:~%s$ ",user_name,host_name,nd);
}




int main()
{
	give_display();
	current_directory = malloc(1000*sizeof(char));	getcwd(current_directory,1000); 
	while(1)
	{
		no_of_commands =0;
		print_display();
		char *input;
		char **commands;
		char **arguments;
		input=get_input(input);
		commands=split_input(input);
		char *point;
		for(int num=0;num<no_of_commands;num++)
		{
			arguments=split_command(commands[num]);
			run_command(arguments);
			free(arguments);
		}
	}

	return 0;
}



/*
void printall(char *input, char *cmnd, char **arguments)
{
	printf("input => %s command=> %s",input,cmnd);
	char *i;
	while(i!=NULL)
	{
	
	}
}*/


/*
void split_command(char ip[], char arguments[][1000])
{
	char *token;
	token = ip;
	token = strtok(token,&delm);
	long long i=0;
	while (token)
	{
		arguments[i] = token;i++;
		token = strtok(NULL,&delm);
	}
	return;
	
}

int split_input(char input[], char commands[][1000])
{
	char *token; 
	token = input;
	token = strtok(token,&delm);
	long long i=0;
	while (token)
	{
		commands[i]=token;i++;
		token = strtok(NULL,&delm);
	}
	return i;

}
*/



