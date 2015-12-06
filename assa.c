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
#include <ctype.h>

#define BUFFER_SIZE 1024

#define errorWrongUsage "[ERR] usage: ./assa [-e brainfuck_filnename]\n"
#define errorReadingFile "[ERR] reading the file failed\n"
#define errorWrongParameterCount "[ERR] wrong parameter count\n"
#define errorNoFileLoaded "[ERR] no program loaded\n"
#define errorOutOfMemory "[ERR] out of memory\n"
#define errorParsingFailed "[ERR] parsing of input failed\n"

typedef struct _Node_
{
    char character;
    char is_break;
    int position;
    struct _Node_ *next;
    struct _Node_ *begin;
    struct _Node_ *end;
} Node;

void free_list(Node *list)
{
  Node *current_pos = list;
  Node *next_pos;

  while (current_pos != NULL)
  {
    next_pos = current_pos->next;
    free(current_pos);
    current_pos = next_pos;
  }
}

// creating linked list for brainfuck instructions
Node *create_list(int num_of_elements)
{
  int counter;
  Node *head = malloc(sizeof(Node));
  if (head == NULL)
  {
    printf(errorOutOfMemory);
    return NULL;
  }

  Node *current_pos = head;

  for(counter = 0; counter <= num_of_elements; counter++)
  {
    Node *new = malloc(sizeof(Node));
    if (new == NULL)
    {
      printf(errorOutOfMemory);
      free_list(head);
      return NULL;
    }

    new->character = 0;
    new->is_break = 0;
    new->position = counter;
    new->next = NULL;
    new->begin = NULL;
    new->end = NULL;

    current_pos->next = new;
    current_pos = new;
  }

  return head;
}


void load_list(Node *list, char *bf_program, unsigned int memory_size)
{
  int counter;
  int bracket_counter = 0;
  Node* brackets[memory_size/2]; // maximal die hälfte des programs sind "["

  for (counter = 0; counter <= memory_size; counter++)
  {
    list->character = bf_program[counter];
    if (bf_program[counter] == '[')
    {
      brackets[bracket_counter++] = list; // speichern "[" in Array zum loopen
    }
    else if (bf_program[counter] == ']')
    {
      list->begin = brackets[--bracket_counter]; // zeigt auf "["
      list->begin->end = list; // "[" zeigt auf "]"
    }

    list = list->next;
  }
}


int loadBfProgram(char **bf_program, char *bf_prog_name,
  unsigned int *memory_size, int *command_count)
{
  FILE *bf_file_ptr = fopen(bf_prog_name, "r");
  if (bf_file_ptr == NULL)
  {
    // file open error
    printf(errorReadingFile);
    return 1;
  }
  else
  {
    char next_char;
    char *realloc_ptr;
    int memory_counter = 0;
    int bracket_counter = 0;

    // save bf program
    while ((next_char = fgetc(bf_file_ptr)) != EOF)
    {
      // realloc memory if needed
      if (memory_counter == *memory_size)
      {
        *memory_size *= 2;

        realloc_ptr = realloc(*bf_program, *memory_size);
        if (realloc_ptr == NULL)
        {
          // return 2 for out of memory
          fclose (bf_file_ptr);
          return 2;
        }

        *bf_program = realloc_ptr;
      }

      if (next_char == '<' || next_char == '>' || next_char == '+' || 
        next_char == '-' || next_char == '.' || next_char == ',' || 
          next_char == '[' || next_char == ']')
      {
        (*bf_program)[memory_counter++] = next_char;
      }

      if (next_char == '[')
      {
        bracket_counter++;
      }
      else if (next_char == ']')
      {
        bracket_counter--;
      }
    }

    // count correct number of bf loops
    if (bracket_counter != 0)
    {
      printf(errorParsingFailed);
      fclose (bf_file_ptr);
      return 1;
    }
    else
    {
      fclose (bf_file_ptr);
      *command_count = --memory_counter;
      return 0;
    }    
  }
}

void resetBfProgramData(char **bf_program)
{
  // calloc default memory for bf prog data
  *bf_program = calloc(BUFFER_SIZE, sizeof(char));
  if (*bf_program == NULL)
  {
    printf(errorOutOfMemory);
    exit(2);
  }
}

// TODO: temporary function for printing array, remove if not needed
void printBfProg(char *bf_program)
{
  unsigned int print_counter;

  for (print_counter = 0; bf_program[print_counter] != '\0'; ++print_counter)
  {
    printf("%c", bf_program[print_counter]);
  }
  printf("\n");
}

void interpret(Node *list, unsigned char **data_memory_in, int interactive)
{
  // TODO: this is ugly

  int shift_right_counter = 0;
  unsigned char *data_memory = *data_memory_in;
  unsigned char *start = data_memory;
  unsigned char *temp_memory;
  Node *temp_next;

  while(list->next != NULL)
  {
    if((shift_right_counter+1 % BUFFER_SIZE) == 0)
    {
      temp_memory = realloc(*data_memory_in, BUFFER_SIZE);
      if(temp_memory == NULL)
      {
        printf(errorOutOfMemory);
        free(*data_memory_in);
        return;
      }
      else
      {
        *data_memory_in = temp_memory;
      }
    }

    temp_next = list->next;
    if(interactive == 1 && list->is_break == 1 )
    {
      return;
    }
    else {
      switch (list->character) {
        case ('>'):
          shift_right_counter++;
              data_memory++;
              break;
        case ('<'):
          if (data_memory != start) (data_memory)--;
              break;
        case ('+'):
          (*data_memory)++;
              break;
        case ('-'):
          (*data_memory)--;
              break;
        case ('.'):
          putchar(*data_memory);
              break;
        case (','):
          *data_memory = (unsigned char) getchar();
              break;
        case ('['):
          if (*data_memory == 0) temp_next = list->end->next;
              break;
        case (']'):
          if (*data_memory != 0) temp_next = list->begin->next;
              break;
        default:
          printf("Shit is on fire!\n");
      }
    }

    list = temp_next;
  }
}



void setBreak(Node *list, int breakpoint)
{
  int ctn = 0;
  while(list != NULL)
  {
    printf("%d\n",ctn++);
    if(list->position == breakpoint) list->is_break = 1;
    list = list->next;
  }
}

int main(int argc, const char *argv[])
{
  Node *list = NULL;
  char command[BUFFER_SIZE] = "default";
  char bf_file_name[128];
  char *bf_program = NULL;
  char *command_splits = "default";
  char *user_input_parameter_one = "default";
  char *user_input_parameter_two = "default";
  char *user_input_parameter_three = "default";

  unsigned int memory_size = BUFFER_SIZE;
  unsigned int show_size_counter;
  unsigned int step_counter;
  unsigned char *data_memory;

  int print_counter = 0;
  int function_error = 0;
  int already_run = 0;
  int command_count = 0;
  int interactive = 0;

  data_memory = calloc(BUFFER_SIZE, sizeof(unsigned char));
  if (data_memory == NULL)
  {
    printf(errorOutOfMemory);
    exit(2);
  }

  // ckeck parameter count for program mode
	if (argc == 1) // interactive debug mode ------------------------------------
	{
      interactive  = 1;
	}
	else if (argc == 3) // run .bf program and quit -----------------------------
	{
        // check flag
        if (strcmp(argv[1], "-e") != 0)
        {
          printf(errorWrongUsage);
          free(data_memory);
          exit(1);
        }

        // calloc space for BF program
        resetBfProgramData(&bf_program);

        // load bf prog
        strcpy(bf_file_name, argv[2]);
        function_error = loadBfProgram(&bf_program, bf_file_name, &memory_size,
          &command_count);
        if (function_error == 1)
        {
          // error in readin file
          free(bf_program);
          free(data_memory);
          exit(4);
        }
        else if (function_error == 2)
        {
          // error out of memory
          printf(errorOutOfMemory);
          free(bf_program);
          free(data_memory);
          exit(2);
        }

        // create linked list
        list = create_list(command_count);
        if (list == NULL)
        {
          // free memory
          free(bf_program);
          free(data_memory);
          exit(2);
        }

        load_list(list, bf_program, command_count);
        interpret(list, &data_memory, interactive);


        //free memory
        free(bf_program);
        free(data_memory);
        free_list(list);

        exit(0);
	}
  else
  {
    printf(errorWrongUsage);
    exit(1);
  }

  // this block handles user input parameters // ------------------------------
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

    // check whether input was empty
    if (strcmp(command, "\n") == 0)
    {
      printf(errorWrongParameterCount);
    }

    // load command
    if (strcmp(user_input_parameter_one, "load") == 0
      && strcmp(user_input_parameter_two, "default") != 0)
    {
      // free last allocated memory if not first
      if (bf_program != NULL)
      {
        free(bf_program);
      }
      
      // reset memory
      resetBfProgramData(&bf_program);

      // load program and parse
      int cmdd_count;
      function_error = loadBfProgram(&bf_program, user_input_parameter_two, 
        &memory_size, &cmdd_count);
      if (function_error == 1)
      {
        free(bf_program);

        // reset values
        bf_program = NULL;
        function_error = 0;
        already_run = 0;
      }
      else
      {
        already_run = 0;
      }
    }
    else if (strcmp(user_input_parameter_one, "load") == 0)
    {
      printf(errorWrongParameterCount);
    }

    // run command
    if (strcmp(user_input_parameter_one, "run") == 0)
    {
      //TODO: if no program loaded error
      if(bf_program == NULL || already_run == 1)
      {
        // no prog loaded error
        printf(errorNoFileLoaded);
      }
      else
      {
        // run bf prog // print as of now
        interpret(list, &data_memory, interactive);
        already_run = 1;
      }
    }

    // eval command
    if (strcmp(user_input_parameter_one, "eval") == 0
      && strcmp(user_input_parameter_two, "default") != 0) // -----------------
    {
      printf("eval.\n");
    }
    else if (strcmp(user_input_parameter_one, "eval") == 0)
    {
      printf(errorWrongParameterCount);
    }

    // break command
    if (strcmp(user_input_parameter_one, "break") == 0
      && strcmp(user_input_parameter_two, "default") != 0) // -----------------
    {
      // check if no program loaded error
      if(bf_program == NULL)
      {
        // no prog loaded error
        printf(errorNoFileLoaded);
      }
      else
      {
        int i;
        int param = 0;
        int bool = 1;
        for (i = 0; i < strlen(user_input_parameter_two); i++)
        {
          if (isdigit(user_input_parameter_two[i]) == 0)
          {
            bool = 0;
            break;
          }
        }
        if (bool == 1)
        {
          param = atoi(user_input_parameter_two);
          setBreak(list, param);
          printf("Break point set at %d \n", param);
        }
      }
    }
    else if (strcmp(user_input_parameter_one, "break") == 0)
    {
      printf(errorWrongParameterCount);
    }

    // step command
    if (strcmp(user_input_parameter_one, "step") == 0) // ---------------------
    {
      // check if no program loaded error
      if(bf_program == NULL)
      {
        // no prog loaded error
        printf(errorNoFileLoaded);
      }
      else
      {
        // set default step size
        step_counter = 1;

        // set to size if entered
        if (strcmp(user_input_parameter_two, "default") != 0)
        {
          step_counter = atoi(user_input_parameter_two);
        }

        // TODO: run X number of steps
        printf("Step counter: %i\n", step_counter);
      }
    }

    // memory command
    if (strcmp(user_input_parameter_one, "memory") == 0) // -------------------
    {
      // check if no program loaded error
      if(bf_program == NULL)
      {
        // no prog loaded error
        printf(errorNoFileLoaded);
      }
      else
      {
        // Default values: number = aktuelle Position; type = hex.
        // TODO: set default memory number to ptr head

        // check for user input
        if (strcmp(user_input_parameter_two, "default") != 0 && 
          strcmp(user_input_parameter_three, "default") != 0)
        {
          // TODO: set memory number to ptr head
        }

        printf("Memory\n");
      }
    }

    // show command
    if (strcmp(user_input_parameter_one, "show") == 0) // ---------------------
    {
      // check if no program loaded error
      if(bf_program == NULL)
      {
        // no prog loaded error
        printf(errorNoFileLoaded);
      }
      else
      {
        // set default size
        show_size_counter = 10;
        // set to size if entered
        if (strcmp(user_input_parameter_two, "default") != 0)
        {
          show_size_counter = atoi(user_input_parameter_two);
        }

        // TODO: jump to ptr head
        for (print_counter = 0; print_counter < show_size_counter && 
          bf_program[print_counter] != '\0'; ++print_counter)
        {
          printf("%c", bf_program[print_counter]);
        }

        printf("\n");
      }
    }

    // change command
    if (strcmp(user_input_parameter_one, "change") == 0
      && strcmp(user_input_parameter_three, "default") != 0) // ---------------
    {
      // check if no program loaded error
      if(bf_program == NULL)
      {
        // no prog loaded error
        printf(errorNoFileLoaded);
      }
      else
      {
        printf("Change.\n");
      }
    }
    else if (strcmp(user_input_parameter_one, "change") == 0 &&
      strcmp(user_input_parameter_three, "default") == 0) // ------------------
    {
      printf(errorWrongParameterCount);
    }
  }

  // prints quit
  if (strcmp(command, "quit") == 0)
  {
    printf("Bye.\n");
  }

	// TODO: free all the memory!
  free(bf_program);
  free(data_memory);

	return 0;
}