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

void errorWrongUsage()
{
  printf("[ERR] usage: ./assa [-e brainfuck_filnename]\n");
}

void errorReadingFile()
{
  printf("[ERR] reading the file failed\n");
}

void errorWrongParameterCount()
{
  printf("[ERR] wrong parameter count\n");
}

void errorNoFileLoaded()
{
  printf("[ERR] no program loaded\n");
}

void errorOutOfMemory()
{
  printf("[ERR] out of memory\n");
}

int loadBfProgram(char **bf_program, char *bf_prog_name, 
  unsigned int *memory_size)
{
  FILE *bf_file_ptr = fopen(bf_prog_name, "r");
  if (bf_file_ptr == NULL)
  {
    // file open error
    return 1;
  }
  else
  {
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
    return 0;
  }
}

void resetBfProgramData(char **bf_program)
{
  // calloc default memory for bf prog data
  *bf_program = calloc(BUFFER_SIZE, sizeof(char));
  if (*bf_program == NULL)
  {
    errorOutOfMemory();
    exit(2);
  }
}

// temp function for printing array
void printBfProg(char *bf_program)
{
  unsigned int print_counter;

  for (print_counter = 0; bf_program[print_counter] != '\0'; ++print_counter)
  {
    printf("%c", bf_program[print_counter]);
  }
  printf("\n");
}

int main(int argc, const char *argv[])
{
  char command[BUFFER_SIZE];
  char bf_file_name[128];
  char *bf_program = NULL;
  char *command_splits = "default";
  char *user_input_parameter_one = "default";
  char *user_input_parameter_two = "default";
  char *user_input_parameter_three = "default";

  unsigned int memory_size = BUFFER_SIZE;

  int function_error = 0;

  // ckecks parameter count for program mode 
	if (argc == 1)
	{
    // interactive debug mode -------------------------------------------------
	}
	else if (argc == 3)
	{
    // run .bf program and quit -----------------------------------------------

    // check flag
    if (strcmp(argv[1], "-e") != 0)
    {
      errorWrongUsage();
      exit(1);
    }

    // calloc space for BF program
    resetBfProgramData(&bf_program);

    // load bf prog
    strcpy(bf_file_name, argv[2]);

    function_error = loadBfProgram(&bf_program, bf_file_name, &memory_size);
    if(function_error == 1)
    {
      // error in readin file
      errorReadingFile();
      free(bf_program);
      exit(4);
    }

    // TODO: print bf prog as of now test
    printBfProg(bf_program);

    //free memory
    free(bf_program);
    exit(0);
	}
  else
  {
    errorWrongUsage();
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

    // check whether input was empty
    if (strcmp(command, "\n") == 0)
    {
      errorWrongParameterCount();  
    }

    // load command
    if (strcmp(user_input_parameter_one, "load") == 0 
      && strcmp(user_input_parameter_two, "default") != 0)
    {
      // reset memory
      resetBfProgramData(&bf_program);

      // load program
      function_error = loadBfProgram(&bf_program, user_input_parameter_two, 
        &memory_size);
      if (function_error == 1)
      {
        errorReadingFile();
        free(bf_program);

        // reset values
        bf_program = NULL;
        function_error = 0;
      }
      else
      {
        // parse bf program
      }
    }
    else if (strcmp(user_input_parameter_one, "load") == 0)
    {
      errorWrongParameterCount();
    }

    // run command
    if (strcmp(user_input_parameter_one, "run") == 0)
    {
      //TODO: if no program loaded error
      if(bf_program == NULL)
      {
        // no prog loaded error
        errorNoFileLoaded();
      }
      else
      {
        // run bf prog // print as of now
        printBfProg(bf_program);
      }
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
      // check if no program loaded error
      if(bf_program == NULL)
      {
        // no prog loaded error
        errorNoFileLoaded();
      }
      else
      {
        printf("Break.\n");
      }
    }
    else if (strcmp(user_input_parameter_one, "break") == 0)
    {
      errorWrongParameterCount();
    }
    
    // step command
    if (strcmp(user_input_parameter_one, "step") == 0 
      && strcmp(user_input_parameter_two, "default") != 0)
    {
      // check if no program loaded error
      if(bf_program == NULL)
      {
        // no prog loaded error
        errorNoFileLoaded();
      }
      else
      {
        printf("Step.\n");
      }
    }
    else if (strcmp(user_input_parameter_one, "step") == 0)
    {
      errorWrongParameterCount();
    }

    // memory command
    if (strcmp(user_input_parameter_one, "memory") == 0 
      && strcmp(user_input_parameter_three, "default") != 0)
    {
      // check if no program loaded error
      if(bf_program == NULL)
      {
        // no prog loaded error
        errorNoFileLoaded();
      }
      else
      {
        printf("Memory.\n");
      }
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
      // check if no program loaded error
      if(bf_program == NULL)
      {
        // no prog loaded error
        errorNoFileLoaded();
      }
      else
      {
        printf("Show.\n");
      }
    }
    else if (strcmp(user_input_parameter_one, "show") == 0)
    {
      errorWrongParameterCount();
    }

    // change command
    if (strcmp(user_input_parameter_one, "change") == 0
      && strcmp(user_input_parameter_three, "default") != 0)
    {
      // check if no program loaded error
      if(bf_program == NULL)
      {
        // no prog loaded error
        errorNoFileLoaded();
      }
      else
      {
        printf("Change.\n");
      }
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