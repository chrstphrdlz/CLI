//Christopher Deleuze
//Operating Systems CLI

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/wait.h>
void readLine(char* input);
int startsWith(char* string, char* begin);
int run(char*info, int background);
char* trimFromFront(char* string, char unwanted);
void brutallyExecute(char* deathScentence);
char**getArgs(char*string);
int main()
{
    char input[200];
	pid_t currPid;
	char* endOfFirstWord, *endOfSecondWord;
	char temp[200];
	char command[200];
	char args[200];
	printf("Started\n");
	readline(input);
	int terminate;

	//by here the line has been parsed, and will continue to be read in as long as the user has not quit
	//(by typing "quit")
	while(!startsWith(input, "quit\0"))
	{
		terminate=0;
		if(startsWith(input, "run \0"))
		{
		    //runs a command (takes off the beginning 4 characters)
			if(run(input+4, 0))
				terminate=1;

		}else if (startsWith(input, "background \0"))
		{
			if(run(input+11, 1))
				terminate=1;

		}else if (startsWith(input, "murder \0"))
		{
			brutallyExecute(input+7);

		}else if (input[0]==0x00)
		{

		}
		else
		{
			printf("\nNot a valid command\n");
		}

		if(terminate)
		{
			return 0;
		}

		readline(input);
	}

	return 0;

}

//must free pointer when done
char** getArgs(char*bigString)
{
	char **args;
	args = malloc(sizeof(char*)*100);
	int i;
	for(i=0;i<100;i++)
	{
		args[i] = malloc(sizeof(char)*100);
	}
	for(i=0;i<100;i++)
	{
		memset(args[i], 0x00, sizeof(char)*100);
	}
	char*stringtemp = bigString;
	char otherString[100];
	int index =0;
	while(*stringtemp!=NULL)
	{
		stringtemp = trimFromFront(stringtemp, ' ');
		if(stringtemp==NULL) break;

		sscanf(stringtemp,"%s", otherString);
		stringtemp+=strlen(otherString);

		strcpy(args[index], otherString);
		index++;

	}
	free(args[index]);
	args[index] = NULL;

	return args;


}
void brutallyExecute(char* deathScentence)
{
	pid_t deadmanWalking;
	deathScentence = trimFromFront(deathScentence, ' ');
	if(deathScentence==NULL || !isdigit(*deathScentence))
	{
		printf("%s Incorrect input \n", deathScentence);
		return;
	}
	sscanf(deathScentence,"%u", &deadmanWalking);
	if(kill(deadmanWalking, SIGKILL))
	{
		printf("Failed to terminate process\n");
	}
	else
	{
		printf("Process terminated\n");
	}
}

//checks if the current line begins with a certain string
int startsWith(char* string, char* begin)
{
	char temp[200];

	memset(temp, 0x00, 200);

	strncpy(temp, string, strlen(begin));

	if(strcmp(temp, begin)==0)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

//return 1 if need to terminate
int run(char* thingToRun, int background)
{
	thingToRun = trimFromFront(thingToRun, ' ');
	char**args;
	char command[200];
	int terminate = 0;
	int *status;
	int i;

	if(thingToRun == NULL)
	{
		printf("Nothing to run\n");
		return terminate;
	}
	else
	{
		//Will run command

		args = getArgs(thingToRun);

		pid_t pid;
		if(pid = fork())
		{
			if(!background)
			{
				if(waitpid(pid,status,0)==-1)
				{
					printf("Error waiting for process to terminate!\n");
				}
			}
			else printf("%d\n", pid);
		}
		else
		{
			if(execv(args[0],args))
			{
				printf("Failed command:%s\n", thingToRun);
				terminate = 1;
			}
			else
			{
				printf("Sucessfully ran command\n");
				if(!background)
				{
					terminate = 1;
				}
			}

			//Will free pointers
			for(i=0;i<100;i++)
			{
				if(args[i]!=NULL) free(args[i]);
			}

			free(args);
		}
	}
	return terminate;
}

//Will advance pointer until not longer points to unwanted char, will return null if only char
char* trimFromFront(char* string, char unwanted)
{
	while(*string==unwanted)
	{
		string++;
	}

	if(*string==NULL)
		return NULL;

	return string;
}
void readline(char input[])
{
	printf("# ");
	fgets(input, 200, stdin);
	input[strlen(input)-1]='\0';

	return;
}
