
/*
  __File = shell.c

  __Author__ = Arin Grigoras

  __Date__ = 14/3/2021

  __Credits__ = https://github.com/brenns10/lsh


*/




//INCLUDES
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <errno.h>



//GLOBAL VARIABLES
char name[25];




//PROTOTYPES

int shell_cd(char **args);
int shell_help(char **args);
int shell_cls(char **args);
int shell_dog(char **args);
int shell_frem(char **args);
int shell_fmk(char **args);
int shell_copy(char **args);
int shell_hostnm(char **args);
int shell_path(char **args);
int shell_hd(char **args);
//int shell_tl(char **args);
int shell_time(char **args);
int shell_exit(char **args);



//DATA

char *builtin_str[] = {
  "cd", //change directory
  "help", //help command
  "cls", //clearscreen command
  "dog", //cat command but with another name
  "frem", //file remove
  "fmk", //file make
  "copy", //copy file command
  "hostnm", //hostname(prints out the host name)
  "path",//path (prints out the current directory)
  "hd", //head(prints out the first line in a file)
  //"tl",
  "time", //time(prints out the current time)
  "exit" //exit(halts the program)
};





int (*builtin_func[]) (char **) = {
  &shell_cd,
  &shell_help,
  &shell_cls,
  &shell_dog,
  &shell_frem,
  &shell_fmk,
  &shell_copy,
  &shell_hostnm,
  &shell_path,
  &shell_hd,
  //&shell_tl,
  &shell_time,
  &shell_exit
};





//FUNCTIONS
int shell_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}




//change directory command
int shell_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "shell: expected argument to \"cd\"\n");
    return 1;
  } else {
    if (chdir(args[1]) != 0) {
      perror("shell");
    }
  }
  return 1;
}




//recreation of the 'clear' command
int shell_cls(char **args){
  printf("\e[1;1H\e[2J");

  return 1;
}




//recreation of the 'cat' command
int shell_dog(char **args){

  if(args[1] == NULL){
    fprintf(stderr, "\n\nshell: please provide a file\n\n");
    return 1;
  }

  char c;
  FILE *fptr = fopen(args[1], "r");

  if(!fptr){
      perror("shell");
      return 1;
  }

  c = fgetc(fptr);
  while(c != EOF){
      printf("%c", c);
      c = fgetc(fptr);
  }

  fclose(fptr);

  return 1;
}




//recreation of the 'rm' command
int shell_frem(char **args){

  if(args[1] == NULL){
    fprintf(stderr, "\n\nshell: please provide a file\n\n");
    return 1;
  }

  if(remove(args[1]) == 0){
    printf("\nDeleted %s succesfully\n\n", args[1]);
  }

  else{
    fprintf(stderr, "\n\nshell: Unable to delete %s\n\n", args[1]);
    return 1;
  }


  return 1;

}




//recreation of the 'touch' command
int shell_fmk(char **args){

  if(args[1] == NULL){
    fprintf(stderr, "\n\nshell: please provide a name for the file\n\n");
    return 1;
  }

  FILE *fptr;

  fptr = fopen(args[1], "w");

  if(!fptr){
    fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
    return 1;
  }


  fclose(fptr);


  return 1;
}




//recreation of the 'cp' command  
int shell_copy(char **args){


  //If there isn't a file specified it returns to the loop
  if(args[1] == NULL){
    fprintf(stderr, "\n\nshell: please provide a file\n\n");
    return 1;
  }


  if(args[2] == NULL){
    fprintf(stderr, "\n\nshell: please provide a file\n\n");
    return 1;
  }


   char ch;
   FILE *source, *target;

   source = fopen(args[1], "r");

   //if the file can't be opened
   if (!source){
      fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
      return 1;
   }

   target = fopen(args[2], "w");

   //if the file can't be opened
   if (!target){
      fclose(source);
      fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
      return 1;
   }

   //copies the first file into the second file
   while ((ch = fgetc(source)) != EOF){
      fputc(ch, target);
   }

   //gives the user confirmation that the command worked
   printf("\nFile copied successfully.\n\n");


   //closes the files
   fclose(source);
   fclose(target);

   return 1;
}




//recreation of the 'hostname' command
int shell_hostnm(char **args){

  printf("\n\n%s\n\n", name);

  return 1;
}


  

//recreation of the 'pwd' command
int shell_path(char **args){

  char cwd[PATH_MAX];
  //gets the curent working directory
  getcwd(cwd, sizeof(cwd));
  //and prints it
  printf("\n%s\n\n", cwd);

  return 1;
}





//recreation of the 'head' command
int shell_hd(char **args){

  char *c = (char*)malloc(sizeof(char) * 100);

  //If there isn't a file specified it returns to the loop
  if(args[1] == NULL){
    fprintf(stderr, "\n\nshell: please provide a file\n\n");
    return 1;
  }

  FILE *fptr;

  fptr = fopen(args[1], "r");

  //If the file can't be opened
  if(!fptr){
    fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
    return 1;
  }


  if(!c){
    fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
    return 1;
  }


  fscanf(fptr, "%[^\n]", c);
  printf("%s", c);


  //frees the 'c' variable and closes the file
  free(c);
  fclose(fptr);

  return 1;
}



int shell_time(char **args){
  time_t rawtime;
  struct tm * timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  printf("Current local date and time: %s", asctime(timeinfo));
}




//help command
int shell_help(char **args){
  int i;
  printf("Arn shell\n");
  printf("The following are built in:\n");

  //loops through every command in the array
  for (i = 0; i < shell_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  return 1;
}





//exit command
int shell_exit(char **args){
  //exits the whole program
  return 0;
}





//launches the commands using pid
int shell_launch(char **args){
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("shell");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("shell");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}





//executes the command
int shell_execute(char **args){
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  //loops through every function in the array
  for (i = 0; i < shell_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return shell_launch(args);
}





#define shell_RL_BUFSIZE 1024



//takes in whatever the user inputed
char *shell_read_line(void){
  int bufsize = shell_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "shell: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {

    c = getchar();

    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }

    position++;

    if (position >= bufsize) {
      bufsize += shell_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "shell: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}





#define shell_TOK_BUFSIZE 64
#define shell_TOK_DELIM " \t\r\n\a"




//parses
char **shell_split_line(char *line){
  int bufsize = shell_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "shell: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, shell_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += shell_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "shell: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, shell_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}




//main loop
void shell_loop(void){
  char *line;
  char **args;
  int status;

  char cwd[PATH_MAX];


  do {
    getcwd(cwd, sizeof(cwd));
    printf("\033[0;32mArn@%s:\033[0;34m%s/\033[0;32m$ ", name ,cwd);
    printf("\033[0m");
    line = shell_read_line();
    args = shell_split_line(line);
    status = shell_execute(args);

    free(line);
    free(args);
  } while (status);
}






int main(int argc, char **argv){

  //loads some files
    
  //GETS THE NAME OF THE USER TO BE USED IN THE MAIN LOOP
  FILE *fptr = fopen("name.txt", "r");

  //ERROR CHECKING
  if(!fptr){
      printf("\n\n\nERR: Couldn't open file\n\n\n");
      strcpy(name, "root");
   }

    //If the file can be opened it does this
  else{
      fseek(fptr, SEEK_END, 0);
      ftell(fptr);
      rewind(fptr);
      //reads the name inside the file into a global variable
      fread(name, 25, 1, fptr);
  }

  // Run command loop.
  shell_loop();


  return EXIT_SUCCESS;
}