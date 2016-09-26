#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
struct command
{
	const char **argv;
};

/*
int check_redirection_with_pipe(char* cmd)
{
	int l=strlen(cmd);
	for(int i=0;i<l;i++)
	{
		if(com[i]=='>')
		{
			return 1;
		}
	}
	return 0;
}*/



int spawn_proc (int in, int out, char *com)
{
	pid_t pid;
	char *par[1000];
	char * Split_temp = strtok(com," "); // strtok to parse the command and storing them in params array
	for(int i = 0; i < 1000; i++) {
		par[i] = Split_temp;
		Split_temp = strtok(NULL," ");
		if(par[i] == NULL) break;
	
	}

	if ((pid = fork ()) == 0)
	{
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

		return execvp (par[0], par);
	}

	return pid;
}
int piping(char * comd)
{
	int save_in, save_out;
	save_in = dup(0);
	save_out = dup(1);
	int i;
	int in, fd [2];
	char * split;
	char com[100][100];
	if(comd[strlen(comd)-1] == '\n')
	{
		comd[strlen(comd)-1] = '\0';
	}
	int temp=0;
	split = strtok(comd,"|"); //seperating different ; seperated commands
	while(split)
	{
		strcpy(com[temp],split);
		split = strtok(NULL,"|");
		temp++;
	}
	in = 0;

	for (i = 0; i < temp-1; ++i)
	{
		pipe (fd);
		spawn_proc (in, fd [1], com[i]);
		close (fd [1]);
		in = fd [0];
	}

	char *par[1000];
	char * Split_temp = strtok(com[i]," "); // strtok to parse the command and storing them in params array
	for(int i = 0; i < 1000; i++)
	{
		par[i] = Split_temp;
		Split_temp = strtok(NULL," ");
		if(par[i] == NULL) break;
	}
	pid_t pid = fork(); // fork process

	if (pid == -1) {
		char* error = strerror(errno);
		printf("fork: %s\n", error);
		dup2(save_in, 0);
		dup2(save_out,1);
		close(save_in);
		close(save_out);
		return 1;
	}

	else if (pid == 0) {
		if (in != 0)
			dup2 (in, 0);
		execvp(par[0], par);
		dup2(save_in, 0);
		dup2(save_out,1);
		close(save_in);
		close(save_out);
		char* error = strerror(errno);
		printf("shell: %s: %s\n", par[0], error);
		return 0;
	}
	else {
		int childStatus;
		waitpid(pid, &childStatus, 0);
		dup2(save_in, 0);
		dup2(save_out,1);
		close(save_in);
		close(save_out);
		return 1;
	}
	dup2(save_in, 0);
	dup2(save_out,1);
	close(save_in);
	close(save_out);
	// close(in);
}
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
