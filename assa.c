//-----------------------------------------------------------------------------
// assa.c
//
// brainfuck interpreter
//
// Group: 2 - study assistant Angela Promitzer
//
// Authors: Adam Ujvari     1232056
// 			    Amel Hamidovic  1330013
//			    Tina Promitzer  1311885
//
// Latest Changes: XX.XX.2015
//-----------------------------------------------------------------------------
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

void errorReadingFile()
{
  printf("[ERR] reading the file failed\n");
}

void errorWrongParameterCount()
{
  printf("[ERR] wrong parameter count\n");
}

void errorOutOfMemory()
{
  printf("[ERR] out of memory\n");
}

// not used TODO: remove if not used
void freeAllTheMemory()
{
}

void loadBfProgram(char **bf_program, const char **argv, 
  unsigned int *memory_size)
{
  FILE *bf_file_ptr = fopen (argv[2], "r");
  if (bf_file_ptr == NULL)
  {
    // file open error
    errorReadingFile();
    free(*bf_program);
    exit(4);
  }

  char next_char;
  char *realoc_ptr;
  int memory_counter = 0;

  // save bf program
  while ((next_char = fgetc(bf_file_ptr)) != EOF)
  {
    // realloc memory if needed
    if (memory_counter == *memory_size)
    {
      *memory_size *= 2;

      realoc_ptr = realloc(*bf_program, *memory_size);
      if (realoc_ptr == NULL)
      {
        errorOutOfMemory();
        free(*bf_program);
        exit(4);
      }

      *bf_program = realoc_ptr;
    }

    (*bf_program)[memory_counter++] = next_char;
  }

  fclose (bf_file_ptr);
}

int main(int argc, const char *argv[])
{
  char command[BUFFER_SIZE];
  char *bf_program;
  char *command_splits = "default";
  char *user_input_parameter_one = "default";
  char *user_input_parameter_two = "default";
  char *user_input_parameter_three = "default";

  unsigned int memory_size = BUFFER_SIZE;
  unsigned int print_counter;

 
  // calloc space for BF program
  bf_program = calloc(memory_size, sizeof(char));
  if (bf_program == NULL)
  {
    errorOutOfMemory();
    exit(2);
  }

  // ckecks parameter count for program mode 
	if (argc == 1)
	{
    // interactive debug mode -------------------------------------------------
		printf("Interaktive debug mode\n");	
	}
	else if (argc == 3)
	{
    // run .bf program and quit -----------------------------------------------
    printf("Program run mode\n");

    // load bf prog
    loadBfProgram(&bf_program, argv, &memory_size);

    // TODO: run as of now test

    for (print_counter = 0; bf_program[print_counter] != '\0'; ++print_counter)
    {
      printf("%c", bf_program[print_counter]);
    }
    printf("\n");

    //free memory
    free(bf_program);
    exit(0);
	}
  else
  {
    printf("[ERR] usage: ./assa [-e brainfuck_filnename]\n");
    exit(1);
  }

  // this block handles user input parameters 
  while (strcmp(command, "quit") != 0)
  {
    // writes prefix, gets user input and checks for EOF
    printf("esp> ");
    if (fgets(command, BUFFER_SIZE, stdin) == NULL)
    {
      printf("\n"); // optional
      break;
    }

    // reset input prameters to default
    user_input_parameter_one = "default";
    user_input_parameter_two = "default";
    user_input_parameter_three = "default";

    // split user input into parameters
    command_splits = strtok(command, " \n");
    if (command_splits != NULL)
    {
      user_input_parameter_one = command_splits;
      command_splits = strtok(NULL, " \n");
      if (command_splits != NULL)
      {
        user_input_parameter_two = command_splits;
        command_splits = strtok(NULL, " \n");
        if (command_splits != NULL)
        {
          user_input_parameter_three = command_splits;
        }
      }
    }

    // printf("%s\n", user_input_parameter_one);
    // printf("%s\n", user_input_parameter_two);
    // printf("%s\n", user_input_parameter_three);

    // check whether input was an empty
    if (strcmp(command, "\n") == 0)
    {
      errorWrongParameterCount();  
    }

    // load command
    if (strcmp(user_input_parameter_one, "load") == 0 
      && strcmp(user_input_parameter_two, "default") != 0)
    {
      //TODO: failed loading, parsed errors
      printf("Load.\n");
    }
    else if (strcmp(user_input_parameter_one, "load") == 0)
    {
      errorWrongParameterCount();
    }

    // run command
    if (strcmp(user_input_parameter_one, "run") == 0)
    {
      //TODO: if no program loaded error
      printf("[ERR] no program loaded\n");
    }

    // eval command
    if (strcmp(user_input_parameter_one, "eval") == 0 
      && strcmp(user_input_parameter_two, "default") != 0)
    {
      printf("eval.\n");
    }
    else if (strcmp(user_input_parameter_one, "eval") == 0)
    {
      errorWrongParameterCount();
    }

    // break command
    if (strcmp(user_input_parameter_one, "break") == 0 
      && strcmp(user_input_parameter_two, "default") != 0)
    {
      //TODO: if no program loaded error
      printf("Break.\n");
    }
    else if (strcmp(user_input_parameter_one, "break") == 0)
    {
      errorWrongParameterCount();
    }
    
    // step command
    if (strcmp(user_input_parameter_one, "step") == 0 
      && strcmp(user_input_parameter_two, "default") != 0)
    {
      //TODO: if no program loaded error
      printf("Step.\n");
    }
    else if (strcmp(user_input_parameter_one, "step") == 0)
    {
      errorWrongParameterCount();
    }

    // memory command
    if (strcmp(user_input_parameter_one, "memory") == 0 
      && strcmp(user_input_parameter_three, "default") != 0)
    {
      //TODO: if no program loaded error
      printf("Memory.\n");
    }
    else if (strcmp(user_input_parameter_one, "memory") == 0 &&
      strcmp(user_input_parameter_three, "default") == 0)
    {
      errorWrongParameterCount();
    }

    // show command
    if (strcmp(user_input_parameter_one, "show") == 0 
      && strcmp(user_input_parameter_two, "default") != 0)
    {
      //TODO: if no program loaded error
      printf("Show.\n");
    }
    else if (strcmp(user_input_parameter_one, "show") == 0)
    {
      errorWrongParameterCount();
    }

    // change command
    if (strcmp(user_input_parameter_one, "change") == 0
      && strcmp(user_input_parameter_three, "default") != 0)
    {
      //TODO: if no program loaded error
      printf("Change.\n");
    }
    else if (strcmp(user_input_parameter_one, "change") == 0 && 
      strcmp(user_input_parameter_three, "default") == 0)
    {
      errorWrongParameterCount();
    }
  }

  // prints quit
  if (strcmp(command, "quit") == 0)
  {
    printf("Bye.\n");
  }

	// TODO: free all the memory!
  free(bf_program);

	return 0;
}