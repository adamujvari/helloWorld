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

void wrongParameterCount()
{
  printf("[ERR] wrong parameter count\n");
}

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
    // interactive debug mode
		printf("Interaktive debug mode\n");	
	}
	else if (argc == 3)
	{
    // run .bf program and quit
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

    // reset input prameters to default
    user_input_parameter_one = "default";
    user_input_parameter_two = "default";
    user_input_parameter_three = "default";
    command_splits = "default";

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
      wrongParameterCount();  
    }

    // load command
    if (strcmp(user_input_parameter_one, "load") == 0 
      && strcmp(user_input_parameter_two, "default") != 0)
    {
      //TODO: failed loading, parsed errors
      printf("Load.\n");
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
      wrongParameterCount();
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
      wrongParameterCount();
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
      wrongParameterCount();
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
      wrongParameterCount();
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
      wrongParameterCount();
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
      wrongParameterCount();
    }
  }

  // prints quit
  if (strcmp(command, "quit") == 0)
  {
    printf("Bye.\n");
  }

	// TODO: free all the memory!

	return 0;
}