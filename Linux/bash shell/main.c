#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<limits.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<dirent.h>
#include <getopt.h>
#include <time.h>
#include <string.h>


int main(int argc, char *argv[])
{     
      //question1
      char* username = getlogin();
      char hostname [HOST_NAME_MAX];
      if(gethostname(hostname, HOST_NAME_MAX) != 0){
            perror("Error getting hostname");
            exit;     
      }
      char cwd[PATH_MAX];
      if(getcwd(cwd,sizeof(cwd))==NULL)
      {
           perror("Error getting current working directory");
           exit;
      }
      
      printf("%s@%s:%s\n", username, hostname, cwd);
        char *command;
        while((command = readline(">")) != NULL){
               add_history(command);
               if(strcmp(command,"exit")==0){
                  free(command);
                  break;
               }
               system(command);
               
        } 
    //ls
    int option;
    char *dir_path;
    DIR *dir;
    struct dirent *entry;
    int hidden_file= 0;
    int long_format=0;
    int reverse_file=0;
    int size_file=0;
    
    while((option=getopt(argc, argv,"alrh")) != -1){
      switch(option){
            case'a':
                hidden_file=1;
                break;
            case'l':
                long_format=1;
                break;
            case'r':
                 reverse_file=1;
                 break;
            case'h':
                 size_file=1;
                 break;
             default:
                 fprintf(stderr,"Use: %s[-a][-l][-r][-h][directory]\n",argv[0]);
                 exit(EXIT_FAILURE);    
      }
     }
     
    if (argc < 2) {
        dir = opendir(".");
    } else {
        dir = opendir(argv[1]);
    }

    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
       //hidden file
        while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.' && option != 'a') {
            continue; 
        }

       else {
            printf("%s\n", entry->d_name);
        }
    }

  /*while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }*/

    closedir(dir);


    
          return 0;
}
