#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

//char input=[100000]
//char arguments=[100000]
//char *token;
long long int no_of_commands;
char dspace[3]=" \n";

void mycd(char *path)
{
	chdir(path);
	return;
}

void mypwd()
{
	char pwd[1000];
	getcwd(pwd,1000);
	printf("%s\n",pwd);
}

void myecho(char **arguments)
{	int i=1;
	char *toprint;
	toprint=arguments[i];
	while(toprint!=NULL)
	{
		printf("%s\n",toprint);
		i++;
		toprint = arguments[i];
	}
}

void run_normal_command(char **arguments)
{
	int pid = fork(),check=0;
	if(pid<0)
	{
		printf("Error Occured\n");
		return ;
	}
	if(pid==0)
	{
		check=execvp(arguments[0],arguments);
		if(check<0)
			printf("ERROR:Command can't be executed\n");
	}
	else
	{
		wait(NULL);
	}
}

void run_command(char **arguments)
{
	char cd[10],pwd[10],echo[10];
	strcpy(cd,"cd");strcpy(pwd,"pwd");strcpy(echo,"echo");
	if(strcmp(arguments[0],cd)==0)	mycd(arguments[1]);
	else if(strcmp(arguments[0],pwd)==0)	mypwd();
	else if(strcmp(arguments[0],echo)==0)	myecho(arguments);
	else	run_normal_command(arguments);
	return ;
}


char *get_input()
{
	char *line = NULL;
	ssize_t bufsize = 0; // have getline allocate a buffer for us
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
		//printf("inserting %s at %lld",arguments[pos-1],pos-1);

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
		//printf("INput => %s at %lld\n",commands[pos],pos);
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


int main()
{
	while(1)
	{
		no_of_commands =0 ;
		printf("$ ");
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



