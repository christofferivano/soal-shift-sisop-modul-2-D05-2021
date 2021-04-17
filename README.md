# soal-shift-sisop-modul-2-D05-2021

## 1. ZIP Stevany
Pada soal ini, kita diminta untuk membuat file zip bernama `Lopyu_Stevany.zip` yang di dalamnya terdapat folder `Musyik`, `Fylm`, dan `Pyoto`, dimana masing-masing folder tersebut berisikan file-file hasil extract zip yang didownload pada `https://drive.google.com/file/d/1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J/view`,  `https://drive.google.com/file/d/1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp/view`, dan `https://drive.google.com/file/d/1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD/view`. File zip ini dibuat dengan 1 script di latar belakang (daemon process) yang berjalan secara otomatis pada 9 April pukul 16.22 WIB untuk membuat folder, mendownload zip, meng-extract zip yang telah didownload, dan memindahkan file-file di dalamnya ke dalam folder yang telah dibuat, serta pada 9 April pukul 22.22 WIB untuk melakukan zip dan menghapus semua folder.

Pertama, kita perlu membuat kerangka daemon process nya terlebih dahulu.
```
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

...

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
    
        ...
        
        sleep(30);        
    }
}
```
Dimana direktori yang digunakan adalah current working directory (`currpath`) dan fungsi `checkfor` digunakan untuk mengecek apakah fork berhasil dilakukan.
```
void checkfork(pid_t child_id){
    if (child_id < 0) {
        exit(EXIT_FAILURE);
    }    
}
```

Selanjutnya, kita perlu menuliskan `if` di dalam `while` agar proses dapat mulai berjalan pada saat 9 April pukul 16.22 WIB, 6 jam sebelum 9 April pukul 22.22 WIB.
```
while (1) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    if (tm->tm_mday==9 && tm->tm_mon==3 && tm->tm_hour==16 && tm->tm_min==22){
    
        ...  
	
    }
    sleep(30);
}
```
Karena `tm_mon` dimulai dari angka 0, kita dapat menuliskan angka 3 untuk mewakili bulan april.

Di dalam `if` kita dapat melakukan `fork` untuk membagi proses menjadi 2, yaitu proses pada saat 9 April pukul 16.22 WIB dan proses pada 9 April pukul 22.22 WIB. Pada proses 9 April pukul 22.22 WIB, kita dapat melakukan loop `while(1)` dengan `sleep(30)` untuk mencari waktu yang diinginkan, yaitu 9 April pukul 22.22 WIB.
```
while (1) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    if (tm->tm_mday==9 && tm->tm_mon==3 && tm->tm_hour==16 && tm->tm_min==22){
        pid_t child_id;
	
	child_id = fork();
	checkfork(child_id);
	
	if (child_id == 0) {
	
	    ...
	    
	} else {
	    while(1) {
	        time_t t2 = time(NULL);
		struct tm *tm2 = localtime(&t2);
		if (tm2->tm_mday==9 && tm2->tm_mon==3 && tm2->tm_hour==22 && tm2->tm_min==22){
		
		    ...
		
		}
                sleep(30);
	    }
        }
    }
    sleep(30);
}
```

Kemudian, di dalam proses 9 April pukul 16.22 WIB, kita dapat membuat 3 proses yang berjalan bersamaan. Ketiga proses itu digunakan untuk memproses musik, film, dan foto secara terpisah.
```
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
```

Pada code di atas, `status1` digunakan untuk memeriksa apakah child proses yang sedang berjalan pada musik sudah selesai atau belum, sedangkan `status2` digunakan untuk film dan `status3` untuk foto. Selain itu, angka `0`, `1`, dan `2` pada actual argument fungsi `runproses` digunakan sebagai index yang akan mengakses array-array yang menyimpan informasi setiap proses seperti berikut ini.
```
char *destdir[] = {"Musyik", "Fylm", "Pyoto"};
char *downloadsrc[] = {"https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download",
                        "https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download",
                        "https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download"};
char *zipname[] = {"Musik_for_Stevany.zip", "Film_for_Stevany.zip", "Foto_for_Stevany.zip"};
char *unzipname[] = {"MUSIK", "FILM", "FOTO"};
```

Fungsi `runproses` berisi rangkaian proses yang akan dilakukan pada saat 9 April pukul 16.22 WIB.
```
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
```
Fungsi `makedir` merupakan fungsi yang digunakan untuk membuat folder, fungsi `download` untuk mendownload zip, fungsi `unzip` untuk meng-extract zip, dan fungsi `movedir` untuk memindahkan file di dalam zip yang telah di-extract ke dalam folder yang telah dibuat oleh fungsi makedir. Proses membuat folder dan mendownload zip dapat dilakukan secara bersamaan, sedangkan proses meng-extract zip perlu menunggu proses mendownload zip dan proses memindahkan file perlu menunggu proses meng-extract zip (dan proses membuat folder).

Untuk membuat folder, kita dapat menuliskan code sebagai berikut pada fungsi makedir.
```
void makedir(char *name){
    char *argv[] = {"mkdir", "-p", name, NULL};
    if((execv("/bin/mkdir", argv)) == -1)
        exit(EXIT_FAILURE);
}
```
Dimana `-p` digunakan untuk mencegah munculnya error ketika folder tersebut sudah ada sebelumnya, dan `name` merupakan nama dari folder yang ingin dibuat, yang telah disimpan pada array `destdir`. Penambahan `if((...) == -1)` pada `execv` digunakan untuk menghentikan proses apabila execv gagal dilakukan.

Kemudian, untuk mendownload zip kita dapat memanfaatkan `wget` dengan menambahkan argumen `--no-check-certificate` untuk mendownload file dari host yang tidak memiliki sertifikat SSL valid, argumen `-O` untuk mengubah nama zip agar sesuai dengan `name`, dan argumen `-o` untuk mengatur output yang dihasilkan karena daemon proses tidak mengizinkan adanya output (`/dev/null`).
```
void download(char *src, char *name){
    char *argv[] = {"wget", "--no-check-certificate", src, "-O", name, "-o", "/dev/null", NULL};
    if((execv("/usr/bin/wget", argv)) == -1)
        exit(EXIT_FAILURE);
}
```
Formal argument `src` merupakan link download zip yang disimpan dalam array `downloadsrc` dan `name` merupakan nama zip yang diinginkan, yang telah tersimpan dalam array `zipname`.

Untuk meng-extract zip yang telah didownload, kita dapat kita dapat menuliskan code sebagai berikut.
```
void unzip(char *name){
    char *argv[] = {"unzip", "-q", name, NULL};
    if((execv("/usr/bin/unzip", argv)) == -1)
        exit(EXIT_FAILURE);
}
```
Argumen `-q` digunakan untuk mencegah unzip menghasilkan output dalam bentuk apapun dan formal argument `name` merupakan nama file zip yang ingin di-extract.

Sedangkan, untuk mencari dan memindahkan file-file pada hasil extract zip ke folder yang telah dibuat, kita dapat memanfaatkan `find` dengan menambahkan argumen `-type` untuk memilih semua yang bertipe file (`f`) dalam path yang masukkan (`srcpath`) dan `-exec` untuk mengeksekusi `mv`, dimana mv memiliki argumen `-t` untuk menuliskan tujuannya (`destpath`).
```
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
```
Variabel `srcpath` dan `destpath` digunakan untuk menyimpan path dari folder hasil extract zip (`src`) dan folder yang telah dibuat sebelumnya (`dest`), dimana src diperoleh dari array `unzipname` dan dest diperoleh dari array `destdir`.

Selanjutnya, untuk melakukan proses zip dan menghapus semua folder, kita dapat membagi proses menjadi 2 proses yang berjalan berurutan pada proses 9 April pukul 22.22 WIB.
```
while (1) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    if (tm->tm_mday==9 && tm->tm_mon==3 && tm->tm_hour==16 && tm->tm_min==22){
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
```
Variabel `status` pada code di atas digunakan untuk memeriksa apakah proses zip sudah selesai atau belum. Untuk melakukan zip, kita dapat memanfaatkan `zip` dengan menambahkan argumen `-rq` agar isi dari setiap folder dapat ikut ter-zip, tidak hanya foldernya saja, dan tidak ada output yang keluar selama proses. Sedangkan, untuk menghapus folder pada currpath, kita dapat menggunakan `find` untuk mencari semua yang bertipe folder (`d`) kecuali folder currpath dengan menambahkan argumen `-mindepth` dan `-type`. Kemudian, untuk menghapus folder yang telah ditemukan, kita dapat menambahkan argumen `-exec` yang akan akan mengeksekusi `rm` secara rekursif (`-r`).

Dengan mengimplementasikan keseluruhan code, kita dapat memperoleh hasil proses pada 9 April pukul 16.22 WIB dan proses pada 9 April pukul 22.22 WIB sebagai berikut, dimana currpath didefinisikan sebagai /home/aulia/Documents/SISOP/Shift2/.
![Hasil proses 9 April pukul 16.22 WIB](https://user-images.githubusercontent.com/76677130/115097018-02521300-9f52-11eb-9ef0-1f919111f793.png)
![hasil proses 9 April pukul 22.22 WIB](https://user-images.githubusercontent.com/76677130/115097022-041bd680-9f52-11eb-9bf6-4fada33207cd.png)

Adapun kendala yang dialami selama pengerjaan soal ini adalah sebagi berikut.
1. Muncul "Segmentation fault (core dumped)" karena kesalahan pengimplementasian fungsi pada library string.h
2. Ketika mencoba menggunakan directory listing dan execv mv, untuk memindahkan file hasil extract zip ke folder yang telah dibuat, terdapat beberapa file yang tidak ikut pindah
3. Program akan berhenti jika terdapat proses yang menghasilkan output

