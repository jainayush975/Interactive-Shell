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

void check1(char mem[], FILE *fp)
{
	int temp =0;
	for(temp=0;temp>=0;temp=temp)
	{
		fgets(mem,100,fp);
		if(strstr(mem,"VmPeak") !=NULL)
		{
			break;
		}
	}

}

void concatenate(char *a,char *b)
{
	strcat(a,b);
}

void mypinfo(char **final)
{
	FILE * fp;
	char mem[100];
	if(final[1] == NULL)
	{
		char proc_path[1024] = "~/./a.out";
		char status[100];
		int pid_curr = getpid();
		fp = fopen ("/proc/self/status", "r");
		char proc_status[100],proc_mem[100],temp4[100],temp5[100];
		int temp = 0;
		fgets(status,100,fp);
		fscanf(fp,"%s %s",status,proc_status);
		check1(mem,fp);

		fscanf(fp, "%s %s",mem,proc_mem);
		printf("Pid : %d\nStatus : %s\nMemory : %s\nExecutable Path : %s\n",pid_curr,proc_status,proc_mem,proc_path);
		fclose(fp);
	}

	else
	{
		char statf[1024] = "/proc/";
		char pathf[1024] = "/proc/";
		char proc_status[100],proc_mem[100];
		concatenate(statf,final[1]);concatenate(pathf,final[1]);concatenate(statf,"/status");concatenate(pathf,"/cmdline");
		size_t buflen = 1024;
		char proc_path[buflen];
		char status[100];
		fp = fopen(pathf,"r");
		fgets(proc_path,1024,fp);
		fclose(fp); 
		fp = fopen (statf, "r");
		char temp5[100];
		fgets(status,100,fp);
		fscanf(fp,"%s %s",mem,proc_status);
		check1(mem,fp);
		fscanf(fp,"%s %s",status,proc_mem);
		printf("Pid : %s\nStatus : %s\nMemory : %s\nExecutable Path : %s\n",final[1],proc_status,proc_mem,proc_path);
		fclose(fp);
	}

}
