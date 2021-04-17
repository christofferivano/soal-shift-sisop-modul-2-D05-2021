#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

char zipPath[] = "/home/amanullahd/Downloads/pets.zip";
char deszipPath[] = "/home/amanullahd/modul2/petshop";
char modul2path[] = "/home/amanullahd/modul2";

void unzipPets();
void makeDir();
void delDir();
void unzipDelet();
int is_regular_file();

int main() {

    
  pid_t child_id;
  int status;

  child_id = fork();

  if (child_id < 0) {
    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
  }

  if (child_id == 0) {
    makeDir(modul2path);
    
  } else {
    // this is parent
    while ((wait(&status)) > 0);
    unzipDelet();
        
  }
  
}

void unzipPets(char *src, char *des){
    char *argv[] = {"unzip", "-q", src, "-d", des, NULL};
    if((execv("/usr/bin/unzip", argv)) == -1)
        exit(EXIT_FAILURE);
}

void makeDir(char *name){
    char *argv[] = {"mkdir", "-p", name, NULL};
    if((execv("/bin/mkdir", argv)) == -1)
        exit(EXIT_FAILURE);
}


void delDir(char *name)
{
    char *argv[] = {"rm", "-r", name, NULL};
    execv("/bin/rm", argv);
}

int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void unzipDelet()
{
    pid_t poin_a;
    int statusa;

    poin_a = fork();

    if (poin_a < 0) {
        exit(EXIT_FAILURE);
    }
    if (poin_a == 0)
        unzipPets(zipPath, deszipPath);
    else
    {
        while ((wait(&statusa)) > 0);
        DIR *dp;
        struct dirent *ep;
        
        dp = opendir(deszipPath);

        if (dp != NULL)
        {
        while ((ep = readdir (dp))) {
            char temp[550];
            sprintf(temp,"%s/%s", deszipPath, ep->d_name);
            if ((strcmp(ep->d_name, ".")==0) || (strcmp(ep->d_name, "..")==0))
                continue;
            if (is_regular_file(temp) == 0){
                pid_t poin_a1;
                poin_a1 = fork();
                int statusa1;
                if (poin_a1 < 0)
                    exit(EXIT_FAILURE);
                if (poin_a1 == 0)
                    delDir(temp);
                
            }
        }
        (void) closedir (dp);
        } else perror ("Couldn't open the directory");
    }

}