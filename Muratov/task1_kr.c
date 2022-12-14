#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void traverse(char *fn, int indent) { // функци, которая пробегается по директории и по всем её субдиректориям и рекурсивно выводит их названия

  DIR* dir;
  struct dirent* entry;
  int count;
  char path[1025];
  struct stat info;

  for (count=0; count < indent; count++) // для понятной распечатки иерархии директорий
    printf("  ");

  printf("%s\n", fn); // вывод названия екущей директории

  if ((dir = opendir(fn)) == NULL)
    perror("opendir() error");         // проверка на ошибку во время открытия директории 

  else {

    while ((entry = readdir(dir)) != NULL) { // дальше мы рекурсивно гуляем по всем директориям

      if (entry->d_name[0] != '.') {

        strcpy(path, fn);
        strcat(path, "/");
        strcat(path, entry->d_name);

        if (stat(path, &info) != 0)
          fprintf(stderr, "stat() error on %s: %s\n", path,
                  strerror(errno));

        else if (S_ISDIR(info.st_mode)) // Макрос S_ISDIR с помощью stat проверяет режим доступа 
               traverse(path, indent+1);
      }
    }

    closedir(dir);
  }
}

int countfiles(char *path) {

    DIR* dir_ptr = NULL;                     // инициализируем необходимые структуры
    struct dirent* direntp;
    char *npath;

    if (!path)                               // проврка на ошибки 
        return 0;

    if( (dir_ptr = opendir(path)) == NULL ) 
        return 0;

    int count=0;

    while( (direntp = readdir(dir_ptr)))
    {
        if (strcmp(direntp -> d_name, ".")==0 || strcmp(direntp -> d_name, "..")==0)  // пропускаем директории "." и ".."
            continue;

        switch (direntp -> d_type) {                                             // проверяем, является ли файл регулярным или это директория, 
                                                                               //в случае директории рекурсивно идём дальше 
            case DT_REG:
                ++count;

                break;

            case DT_DIR:            
                npath=malloc(strlen(path) + strlen(direntp -> d_name) + 2);
                sprintf(npath, "%s/%s", path, direntp -> d_name);

                count += countfiles(npath);
                
                free(npath);

                break;
        }

    }

    closedir(dir_ptr);

    return count;
}

int countfiles_w2links(char *path) { // эта функция работает аналогичным образом, только мы ещё с помощью stat проверяем количество жёстких ссылок

    DIR* dir_ptr = NULL;
    struct dirent* direntp;
    char *npath;

    struct stat buf;

    if (!path) 
        return 0;

    if( (dir_ptr = opendir(path)) == NULL ) 
        return 0;

    int count=0;

    while( (direntp = readdir(dir_ptr)))
    {
        if (strcmp(direntp->d_name,".")==0 || strcmp(direntp->d_name,"..")==0) 
            continue;

        if (direntp -> d_type == DT_REG) {

            stat(direntp -> d_name, &buf);

            if (buf.st_nlink > 1) 
                ++count;

            break;
        }

        else if (direntp -> d_type == DT_DIR) {

            npath=malloc(strlen(path)+  strlen(direntp -> d_name) + 2);
            sprintf(npath, "%s/%s", path, direntp->d_name);

            count += countfiles(npath);
            free(npath);

            break;
        }
    }

    closedir(dir_ptr);

    return count;
}

int main(int argc, char** argv) {

    char* path = argv[1];

    printf("2) Directory structure:\n");
    traverse(path, 0);

    printf("1) Number of files: %d\n", countfiles(path));

    printf("3) Number of files with >= 2 links: %d\n", countfiles_w2links(path));

    return 0;
}