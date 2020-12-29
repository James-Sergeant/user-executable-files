#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

//sets the buffer size for the program.
#define BUFFER_SIZE (256)

void getExecute(char dir[]){
    //A pointer to the open directory
    DIR *dp = opendir(dir);
    if(dp == NULL) printf("%s dose not exist\n",dir);
    else {
        //The struct that contains the different files in that directory, and the data about them.
        struct dirent *entry;
        //The char buffer
        char buffer[BUFFER_SIZE];

        //While there are files to read.
        while ((entry = readdir(dp)) != NULL) {
            //Creates a stat struct
            struct stat sb;
            //Stores the file path in the buffer
            snprintf(buffer, BUFFER_SIZE, "%s/%s", dir, entry->d_name);

            //stores the files stats in sb.
            stat(buffer, &sb);

            //Checks to see if the file is a regular file and the user can execute that file.
            //The logical and of st_mode and the S_IEXEC will be 1 when a file is executable.
            if (S_ISREG(sb.st_mode) != 0 && sb.st_mode & S_IXUSR) {
                //Prints the file if it is executable.
                printf("User can execute: '%s'\n", buffer);
            }
        }

        closedir(dp);
    }
}

int main(int argc, char **argv) {
    //Get the current working directory
    char dir[BUFFER_SIZE];
    //Read in user args
   if(argc == 2){
       //Takes the first argument of the command and uses it to setup the list.
       char buffer[BUFFER_SIZE];
       char *ptr = strtok(argv[1],":");
       getExecute(ptr);

       //Loops through and adds other arguments to the list.
       ptr = strtok(NULL, ":");
       while(ptr != NULL){
           getExecute(ptr);
           ptr = strtok(NULL, ":");
       }
   }else if(argc > 2){
       for(int i = 1; i < argc; i++){
           strcpy(dir,argv[i]);
           getExecute(dir);
       }
   }else{
       //Assigns the current working directory to dir, if an error occurs it will print a message.
       if (getcwd(dir,sizeof(dir))==NULL){
           perror("Error getting working directory");
           return EXIT_FAILURE;
       }
       getExecute(dir);
   }
    return EXIT_SUCCESS;
}
