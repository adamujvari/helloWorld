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

#define COMMAND_BUFFER_SIZE 1024
#define USER_INPUT_PARAM_LENGTH 128

int main(int argc, const char *argv[])
{
  char command[COMMAND_BUFFER_SIZE];
  char *command_splits = "default";
  char *user_input_parameter_one = "default";
  char *user_input_parameter_two = "default";
  char *user_input_parameter_three = "default";

  // ckecks parameter count for program mode
	if (argc == 1)
	{
		printf("Interaktive debug mode\n");	
	}
	else if (argc == 3)
	{
    printf("Program run mode\n");
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
    if (fgets(command, COMMAND_BUFFER_SIZE, stdin) == NULL)
    {
      printf("\n"); // optional
      break;
    }

    // check whether input was an empty line
    if (strcmp(command, "\n") == 0)
    {
      printf("[ERR] wrong parameter count\n");

    }

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

    // run parameter
    if (strcmp(user_input_parameter_one, "run") == 0)
    {
      // TODO: define bf memory struct + check
      printf("[ERR] no program loaded\n");
    }
    
    // reset input prameters to default
    user_input_parameter_one = "default";
    user_input_parameter_two = "default";
    user_input_parameter_three = "default";
    command_splits = "default";
  }

  // prints quit
  if (strcmp(command, "quit") == 0)
  {
    printf("Bye.\n");
  }

	// TODO: free all the memory!

	return 0;
}