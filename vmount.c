#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <sys/wait.h>

#define READ_END 0   
#define WRITE_END 1 

void success(char *container, char *mountpoint)
{
	int status;

	wait(&status);
	if (status == 0) {
		printf("Successfully mounted \t%s\tat %s\n", container, mountpoint);
	} else {
		printf("Error mounting \t%s\tat %s\t<< return value:%d\n",
				container, mountpoint, status);
	}
}

int main(int argc, char* argv[])
{
	if (argc > 1 && argc%2==1) {
		int inst = (argc-1)/2;
		int     fd[inst][2];
		pid_t   pid;
		char    scmd[] = "sudo";

		for (int i = 0; i<inst;i++) {
			pipe(fd[i]);
		}
		pid = fork();

		if (pid==0) {
			int j=0;

			dup2(fd[inst-1][READ_END], STDIN_FILENO);
			close(fd[inst-1][WRITE_END]);
			close(fd[inst-1][READ_END]);

			bool b = true;
			for (int i=0;i<inst-1 && b;i++) {
				pid = fork();
				if (pid==0) {
					j = i+1;
				} else {
					j = i;
					b = false;
					for(int i=0;i<inst-1-j;i++){
						dup2(fd[i][READ_END], STDIN_FILENO);
						close(fd[i][WRITE_END]);
						close(fd[i][READ_END]);
					}
					success(argv[(j+1)*2+1],argv[(j+1)*2+2]);
				}
			}

			freopen("/dev/null", "w", stdout);

			execlp(scmd, scmd, "veracrypt", "-t", "--protect-hidden=no",
					"--keyfiles=", "--pim=753",argv[j*2+1],argv[j*2+2],
					(char*) NULL);
			fprintf(stderr, "Failed to execute '%s'\n", scmd);
			exit(1);
		} else { 
			char *buffer = getpass("Veracrypt Password:");

			for (int i=0;i<inst;i++) {
				close(fd[i][READ_END]);
				write(fd[i][WRITE_END], buffer, (strlen(buffer)+1));
				close(fd[i][WRITE_END]);
			}

			srand(time(0));	
			for (int i = 0; i<strlen(buffer); i++) {
				buffer[i] = rand();
			}

			free(buffer);
			success(argv[1],argv[2]);
		}
	}
	return(0);
}
