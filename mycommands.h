#ifndef __MYLIB__
#define __MYLIB__

void mycd(char *path, char *current_directory);
void myecho(char **arguments);
void mypwd();
void mypinfo(char **final);
void myjobs(char njbs[][100] , int *jbs ,int *flagit, int end);
void mykjobs( pid_t pd , int sig);
void mykillallbg(int *jbs , int *flagit , int end);
void myfg(pid_t pd);

#endif
