#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
void mycd(char *path, char *current_directory)
{
	if((path==NULL)||(path[0]=='~'))
		path=current_directory;
	chdir(path);
	return;
}


void myecho(char **arguments)
{	int i=1;
	char *toprint;
	toprint=arguments[i];
	while(toprint!=NULL)
	{
		printf("%s ",toprint);
		i++;
		toprint = arguments[i];
	}
	printf("\n");
return;
}


void mypwd()
{
	char pwd[1000];
	getcwd(pwd,1000);
	printf("%s\n",pwd);
return;
}

