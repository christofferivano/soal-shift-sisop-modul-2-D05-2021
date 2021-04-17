#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <wait.h>
#include <time.h>
#define MAX_LENGTH 250

char currpath[] = "/home/aulia/Documents/SISOP/Shift2/";
char *destdir[] = {"Musyik", "Fylm", "Pyoto"};
char *downloadsrc[] = {"https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download",
                        "https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download",
                        "https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download"};
char *zipname[] = {"Musik_for_Stevany.zip", "Film_for_Stevany.zip", "Foto_for_Stevany.zip"};
char *unzipname[] = {"MUSIK", "FILM", "FOTO"};

void checkfork(pid_t child_id){
    if (child_id < 0) {
        exit(EXIT_FAILURE);
    }    
}

void makedir(char *name){
    char *argv[] = {"mkdir", "-p", name, NULL};
    if((execv("/bin/mkdir", argv)) == -1)
        exit(EXIT_FAILURE);
}

void download(char *src, char *name){
    char *argv[] = {"wget", "--no-check-certificate", src, "-O", name, "-o", "/dev/null", NULL};
    if((execv("/usr/bin/wget", argv)) == -1)
        exit(EXIT_FAILURE);
}

void unzip(char *name){
    char *argv[] = {"unzip", "-q", name, NULL};
    if((execv("/usr/bin/unzip", argv)) == -1)
        exit(EXIT_FAILURE);
}

void movedir(char *src, char *dest){    
    char srcpath[MAX_LENGTH];
    char destpath[MAX_LENGTH];

    strcpy(srcpath, currpath);
    strcat(srcpath, src);

    strcpy(destpath, currpath);
    strcat(destpath, dest);

    char *argv[] = {"find", srcpath, "-type", "f", "-exec", "mv", "-t", destpath, "{}", ";", NULL};
    if((execv("/usr/bin/find", argv)) == -1)
        exit(EXIT_FAILURE);
}

void runproses(int index, int *status){
    pid_t child_id_a;

    child_id_a = fork();
    checkfork(child_id_a);

    if (child_id_a == 0) {
        // buat folder
        makedir(destdir[index]);
    } else {
        pid_t child_id_b;

        child_id_b = fork();
        checkfork(child_id_b);

        if (child_id_b == 0) {
            // download
            download(downloadsrc[index], zipname[index]);
        } else {
            while ((wait(status)) > 0);
            pid_t child_id_c;

            child_id_c = fork();
            checkfork(child_id_c);

            if (child_id_c == 0) {
                // unzip
                unzip(zipname[index]);
            } else {
                while ((wait(status)) > 0);
                // move
                movedir(unzipname[index], destdir[index]);
            }
        }
    }  
}

int main(){
    pid_t pid, sid;     
    
    pid = fork();     
    checkfork(pid);

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    checkfork(sid);

    if ((chdir(currpath)) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1) {
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        if (tm->tm_mday==9 && tm->tm_mon==3 && tm->tm_hour==16 && tm->tm_min==22){
            pid_t child_id;

            child_id = fork();
            checkfork(child_id);

            if (child_id == 0) {
                int status1, status2, status3;
                pid_t child_id1;

                child_id1 = fork();
                checkfork(child_id1);

                if (child_id1 == 0) {
                    // musik
                    runproses(0, &status1);
                } else {
                    pid_t child_id2;

                    child_id2 = fork();
                    checkfork(child_id2);

                    if (child_id2 == 0) {
                        // film
                        runproses(1, &status2);
                    } else {
                        // foto
                        runproses(2, &status3); 
                    }       
                }      
            } else {
                while(1) {
                    time_t t2 = time(NULL);
                    struct tm *tm2 = localtime(&t2);
                    if (tm2->tm_mday==9 && tm2->tm_mon==3 && tm2->tm_hour==22 && tm2->tm_min==22){
                        int status;
                        pid_t child_id3;

                        child_id3 = fork();
                        checkfork(child_id3);

                        if (child_id3 == 0) {
                            char *argv[] = {"zip", "-rq", "Lopyu_Stevany.zip", destdir[0], destdir[1], destdir[2], NULL};
                            if((execv("/usr/bin/zip", argv)) == -1)
                                exit(EXIT_FAILURE);
                        } else {
                            while ((wait(&status)) > 0);
                            char *argv[] = {"find", currpath, "-mindepth", "1", "-type", "d", "-exec", "rm", "-r", "{}", "+", NULL};
                            if((execv("/usr/bin/find", argv)) == -1)
                                exit(EXIT_FAILURE);
                        }
                    }
                    sleep(30);
                }
            }
        } 
        sleep(30);        
    }
}
