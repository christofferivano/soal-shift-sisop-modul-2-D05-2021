#include <stdio.h>
#include <sys/types.h>
#include <wait.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	//daemon 3d 3e
	pid_t pid, sid;
	pid = fork();
	if (pid < 0) exit(0);
	if (pid > 0) exit(0);
	umask(0);
	sid = setsid();
	if (sid < 0) exit(0);
	if (argc == 2 && strcmp(argv[1], "-z") == 0)
	{
		FILE *file = NULL;
		file = fopen("killer.sh", "w");
		fprintf(file, "#!/bin/bash\npkill soal3\nrm killer.sh\n");
		fclose(file);
	}
	else if (argc == 2 && strcmp(argv[1], "-x") == 0)
	{
		FILE *file = NULL;
		file = fopen("killer.sh", "w");
		fprintf(file, "#!/bin/bash\nkill %d\nrm killer.sh\n", getpid());
		fclose(file);
	}
	//3a 3b 3c
	while(1)
	{
		int flag = 0;
		pid_t cid1, cid2, cid3, cid4, cid5;
		int status = 0;
		time_t tnow;
		time(&tnow);
		struct tm *timenow;
		timenow = localtime(&tnow);
		char timestamp[100];
		strftime(timestamp, 100, "%Y-%m-%d_%X", timenow);
		//char *argv[] = {"mkdir", time, NULL};
		//now = datetime.datetime.now();
		//timestamp = str(now.strftime("%Y%m%d_%H:%M:%S"));
		cid1 = fork();
		if (cid1 < 0) exit(0);
		if (cid1 == 0)
		{
			//char *argv[] = {"mkdir", "/home/christoffer/soal3/"+timestamp, NULL};
			char *argv[] = {"mkdir", timestamp, NULL};
			execv ("/bin/mkdir", argv);
		}
		sleep(1);
		//while(wait(&status) > 0);
		cid2 = fork();
		if (cid2 < 0) exit(0);
		if (cid2 == 0)
		{
			chdir(timestamp);
			for (int i = 0; i < 10; i++)
			{
				time_t tnow2;
				time(&tnow2);
				struct tm * timenow2;
				timenow2 = localtime(&tnow2);
				char timestamp2[100]; 
				timenow2 = localtime(&tnow2);
				strftime(timestamp2, 100, "%Y-%m-%d_%X", timenow2);
				int epoch = (int)tnow2 % 1000 + 50;
				char link[100];
				sprintf(link, "https://picsum.photos/%d", epoch);
				cid3 = fork();
				if (cid3 < 0) exit(0);
				if (cid3 == 0)
				{
					char *argv[] = {"wget", link, "-O", timestamp2, NULL};
					execv ("/bin/wget", argv);
				}
				sleep(5);
				flag++;
			}
		}
		//while(wait(&status) > 0);
		char sukses[] = {"Download Success"};
		for (int i = 0; i < strlen(sukses); i++)
		{
			if (sukses[i] == ' ') continue;
			else if (sukses[i] >= 'A' && sukses[i] <= 'Z')
			{
				if (sukses[i] > 'U')
				{
					sukses[i] = sukses[i] -  21;
					continue;
				}
				sukses[i] = sukses[i] + 5;
			}
			else if (sukses[i] >= 'a' && sukses[i] <= 'z')
			{
				if (sukses[i] > 'u')
				{
					sukses[i] = sukses[i] - 21;
					continue;
				}
				sukses[i] = sukses[i] + 5;
			}
		}
		//while(wait(&status) > 0);
		if (flag == 10)
		{
			FILE *file = NULL;
			file = fopen("status.txt", "w");
			fputs(sukses, file);
			fclose(file);
		}
		//sleep(1);
		//chdir("..");
		cid4 = fork();
		//while (wait(&status) > 0);
		//cid4 = fork();
		if (cid4 < 0) exit(0);
		if (cid4 == 0)
		{
			chdir("..");
			char zip_name[100];
			strcpy(zip_name, timestamp);
			strcat(zip_name, ".zip");
			char *argv[] = {"zip", "-rm", zip_name, timestamp, NULL};
			execv("/bin/zip", argv);
		}
		//if (cid5 < 0) exit(0);
		//if (cid5 == 0)
		//{
			//char *argv[] = {"rm", "-r", timestamp, NULL};
			//execv("/bin/rm", argv);
		//}
		//while (wait(&status) > 0);
		flag = 0;
		sleep(39);
	}
	return 0;
}
