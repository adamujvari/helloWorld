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
#include <math.h>

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

void freeList(Node *list)
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
Node *createList(int num_of_elements)
{
  int counter;
  Node *head = malloc(sizeof(Node));
  if (head == NULL)
  {
    printf(errorOutOfMemory);
    return NULL;
  }

  head->character = 0;
  head->is_break = 0;
  head->position = 0;
  head->next = NULL;
  head->begin = NULL;
  head->end = NULL;

  Node *current_pos = head;

  for(counter = 1; counter <= num_of_elements; counter++)
  {
    Node *new = malloc(sizeof(Node));
    if (new == NULL)
    {
      printf(errorOutOfMemory);
      freeList(head);
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


void loadList(Node *list, unsigned char *bf_program, int memory_size)
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

int parseAndSaveCharacter(char character, unsigned int *memory_size, 
  int *memory_counter, int *bracket_counter, unsigned char **bf_program)
{
  // realloc memory if needed
  if (*memory_counter == (*memory_size) - 1)
  {

    unsigned char *realloc_ptr;

    (*memory_size) *= 2;

    realloc_ptr = realloc(*bf_program, (*memory_size) * sizeof(unsigned char));
    if (realloc_ptr == NULL)
    {
      // return 2 for out of memory
      return 2;
    }

    *bf_program = realloc_ptr;
  }

  if (character == '<' || character == '>' || character == '+' || 
    character == '-' || character == '.' || character == ',' || 
      character == '[' || character == ']')
  {
    (*bf_program)[(*memory_counter)++] = (unsigned char)character;
  }

  if (character == '[')
  {
    (*bracket_counter)++;
  }
  else if (character == ']')
  {
    (*bracket_counter)--;
  }

  return 0;
}

int loadBfProgram(unsigned char **bf_program, char *bf_prog_name,
  unsigned int *memory_size, int *memory_counter)
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
    int function_error = 0;
    int bracket_counter = 0;
    *memory_counter = 0;

    // save bf program
    while ((next_char = fgetc(bf_file_ptr)) != EOF)
    {
      function_error = parseAndSaveCharacter(next_char, memory_size, 
        memory_counter, &bracket_counter, bf_program);
    }
    if (function_error == 2)
    {
      // out of memory error
      fclose(bf_file_ptr);
      return 2;
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
      --(*memory_counter);

      return 0;
    }
  }
}

void callocBfProgramData(unsigned char **bf_program_data)
{
  // calloc default memory for bf prog data
  *bf_program_data = calloc(BUFFER_SIZE, sizeof(unsigned char));
  if (*bf_program_data == NULL)
  {
    printf(errorOutOfMemory);
    exit(2);
  }
}

unsigned char *callocData(unsigned char *array, int old_size, int new_size)
{
  unsigned char *new = calloc(sizeof(unsigned char), (size_t) new_size);
  if(new == NULL)
  {
    return NULL;
  }
  memcpy(new, array, (size_t) old_size);
  free(array);
  return new;
}

int checkIfDigits(char *user_input_parameter)
{
  int counter;
  for (counter = 0; counter < strlen(user_input_parameter); counter++)
  {
    if (isdigit(user_input_parameter[counter]) == 0)
    {
      return 0;
    }
  }
  return 1;
}

int checkIfHex(char *user_input_parameter)
{
  int counter;
  for (counter = 0; counter < strlen(user_input_parameter); counter++)
  {
    if((isxdigit(user_input_parameter[counter]) == 0) ||
            user_input_parameter[counter] == 'x')
    {
      return 0;
    }
  }
  return 1;
}

unsigned char convertToDecimal(char *user_input_parameter)
{
  const int base = 16; // Base of Hexadecimal Number
  long long_number = 0;
  unsigned char decimal_number;

  long_number  = strtol(user_input_parameter, NULL, base);

  if(long_number < 256 && long_number > 0)
  {
    decimal_number = (unsigned char) long_number;
  }
  else
  {
    printf(errorWrongUsage);
    exit(0);
  }

  return decimal_number;
}

void change(int position, unsigned char *data_memory, unsigned char input)
{
  data_memory[position] = input;
}

void interpret(Node **start_node, unsigned char **data_memory_in,
               int *data_memory_size,int interactive, int command_count ,
               int step_counter, int *shift_right_counter)
{
  // TODO: interpret run till breakpoint

  unsigned char *data_memory = *data_memory_in;
  unsigned char *start = data_memory;
  unsigned char *temp_memory;
  Node *temp_next;

  if((*start_node) != NULL)
  {
    if((((*start_node)->position) + step_counter) < command_count)
    {
      step_counter += (*start_node)->position;
    }
    else
    {
      step_counter = command_count;
      printf("End of program reached!\n");
    }
  }


  while ((*start_node) != NULL)
  {
    if(((*start_node)->position == step_counter) ||
            (step_counter > command_count))
    {
      return;
    }
    else
    {
      if (((*shift_right_counter) + 1 % BUFFER_SIZE) == 0)
      {
        temp_memory = callocData(*data_memory_in, *data_memory_size,
                                 (*data_memory_size) * 2);
        *data_memory_size *= 2;
        if (temp_memory == NULL)
        {
          printf(errorOutOfMemory);
          free(*data_memory_in);
          return;
        }
        else {
          *data_memory_in = temp_memory;
          free(temp_memory);
        }
      }

      temp_next = (*start_node)->next;
      if (interactive == 1 && (*start_node)->is_break == 1)
      {
        printf("\n");
        return;
      }
      else {
        switch ((*start_node)->character) {
          case ('>'):
            (*shift_right_counter)++;
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
            if (*data_memory == 0) temp_next = (*start_node)->end->next;
                break;
          case (']'):
            if (*data_memory != 0) temp_next = (*start_node)->begin->next;
                break;
          default:
            printf("Shit is on fire!\n");
        }
      }
      (*start_node) = temp_next;
    }
    (*start_node) = temp_next;
  }
}

void setBreak(Node *list, int breakpoint)
{
  //int counter = 0;
  while(list != NULL)
  {
    if(list->position == breakpoint)
    {
      list->is_break = 1;
      return;
    }
    list = list->next;
  }
}

void printBfInstructions(unsigned char *bf_program)
{
  unsigned int print_counter;

  for (print_counter = 0; bf_program[print_counter] != '\0'; ++print_counter)
  {
    printf("%x", bf_program[print_counter]);
  }
  printf("\n");
}

void printList(Node *list)
{
  while (list != NULL)
  {
    printf("%c", list->character);
    list = list->next;
  }
}

void freeAllTheMemory(unsigned char **bf_program, unsigned char **data_memory, 
  Node **list, unsigned char **eval_memory, Node **eval_list)
{
  if (*bf_program != NULL)
  {
    free(*bf_program);
    *bf_program = NULL;
  }

  if (*data_memory != NULL)
  {
    free(*data_memory);
    *data_memory = NULL;
  }

  if (*list != NULL)
  {
    freeList(*list);
    *list = NULL;
  }

  if (*eval_memory != NULL)
  {
    free(*eval_memory);
    *eval_memory = NULL;
  }

  if (*eval_list != NULL)
  {
    freeList(*eval_list);
    *eval_list = NULL;
  }
}

void inToBinary(unsigned int number, char *binary_value)
{
  int loop_counter;
  binary_value[8] = '\0';

  for (loop_counter = 7; loop_counter >= 0; --loop_counter, number >>= 1)
  {
      binary_value[loop_counter] = (number & 1) + '0';
  }

}

int main(int argc, const char *argv[])
{
  Node *list = NULL;
  Node *eval_list = NULL;
  Node *list_iterator = NULL;
  Node *start_node = NULL;
  
  char command[BUFFER_SIZE] = "default";
  char bf_file_name[128];
  char *command_splits = "default";
  char user_input_parameter_one[BUFFER_SIZE];
  char user_input_parameter_two[BUFFER_SIZE];
  char user_input_parameter_three[BUFFER_SIZE];
  unsigned char change_input = 0;
  char next_char;

  unsigned char *bf_program = NULL;
  unsigned char *eval_memory = NULL;
  unsigned char *data_memory = NULL;

  unsigned int memory_size = BUFFER_SIZE;
  unsigned int show_size_counter;
  unsigned int step_counter;

  int data_memory_size = BUFFER_SIZE;
  int print_counter = 0;
  int function_error = 0;
  int memory_counter = 0;
  int interactive = 0;
  int shift_right_counter = 0;
  int break_point = 0;
  int bracket_counter = 0;
  int memory_id;
  int loop_counter;

  // ckeck parameter count for program mode
	if (argc == 1) // interactive debug mode ------------------------------------
	{
      interactive = 1;
	}
	else if (argc == 3) // run .bf program and quit -----------------------------
	{
      // check flag
      if (strcmp(argv[1], "-e") != 0)
      {
        printf(errorWrongUsage);
        exit(1);
      }

      // calloc space for BF program
      callocBfProgramData(&bf_program);
      callocBfProgramData(&data_memory);

      // load bf prog
      strcpy(bf_file_name, argv[2]);
      function_error = loadBfProgram(&bf_program, bf_file_name, 
        &memory_size,
        &memory_counter);
      if (function_error == 1)
      {
        // error in readin file
        freeAllTheMemory(&bf_program, &data_memory, &list, &eval_memory, 
          &eval_list);
        exit(4);
      }
      else if (function_error == 2)
      {
        // error out of memory
        printf(errorOutOfMemory);
        freeAllTheMemory(&bf_program, &data_memory, &list, &eval_memory, 
          &eval_list);
        exit(2);
      }

      // create linked list
      list = createList(memory_counter);
      if (list == NULL)
      {
        // free memory
        freeAllTheMemory(&bf_program, &data_memory, &list, &eval_memory, 
          &eval_list);
        exit(2);
      }

      loadList(list, bf_program, memory_counter);

      start_node = list;
      interpret(&start_node, &data_memory, &data_memory_size,interactive, 
        memory_size,-1, &shift_right_counter);

      //free memory
      freeAllTheMemory(&bf_program, &data_memory, &list, &eval_memory, 
        &eval_list);

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
    strcpy(user_input_parameter_one, "default");
    strcpy(user_input_parameter_two, "default");
    strcpy(user_input_parameter_three, "default");

    // split user input into parameters
    command_splits = strtok(command, " \n\t");
    if (command_splits != NULL)
    {
      strcpy(user_input_parameter_one, command_splits);
      command_splits = strtok(NULL, " \n\t");
      if (command_splits != NULL)
      {
        strcpy(user_input_parameter_two, command_splits);
        command_splits = strtok(NULL, " \n\t");
        if (command_splits != NULL)
        {
          strcpy(user_input_parameter_three, command_splits);
        }
      }
    }

    // check whether input was empty
    if (strcmp(command, "\n") == 0)
    {
      printf(errorWrongParameterCount);
    }

    // load command -----------------------------------------------------------
    if (strcmp(user_input_parameter_one, "load") == 0
      && strcmp(user_input_parameter_two, "default") != 0)
    {
      // free last allocated memory if not first
      freeAllTheMemory(&bf_program, &data_memory, &list, &eval_memory, 
        &eval_list);
      
      // reset memory
      callocBfProgramData(&bf_program);
      callocBfProgramData(&data_memory);

      // load program and parse
      function_error = loadBfProgram(&bf_program, user_input_parameter_two,
        &memory_size, &memory_counter);
      if (function_error == 1)
      {
        // error in readin file, reset values
        function_error = 0;
      }
      else if (function_error == 2)
      {
        // error out of memory
        printf(errorOutOfMemory);
        freeAllTheMemory(&bf_program, &data_memory, &list, &eval_memory, 
          &eval_list);
        exit(2);
      }
      else
      {
        // create linked list
        list = createList(memory_counter);
        if (list == NULL)
        {
          // free memory
          freeAllTheMemory(&bf_program, &data_memory, &list, &eval_memory, 
            &eval_list);
          exit(2);
        }

        loadList(list, bf_program, memory_counter);
        start_node = list;
      }
    }
    else if (strcmp(user_input_parameter_one, "load") == 0)
    {
      printf(errorWrongParameterCount);
    }

    // run command
    if (strcmp(user_input_parameter_one, "run") == 0)
    {
      // if no program loaded error
      if(list == NULL)
      {
        printf(errorNoFileLoaded);
      }
      else
      {
        // run bf prog
        interpret(&start_node, &data_memory, &data_memory_size,interactive, 
          memory_size ,-1, &shift_right_counter);
      }
    }

    // eval command -----------------------------------------------------------
    if (strcmp(user_input_parameter_one, "eval") == 0
      && strcmp(user_input_parameter_two, "default") != 0)
    {
      // allocate eval instruction memory
      callocBfProgramData(&eval_memory);

      // check whether program data exists, if no create.
      if (data_memory == NULL)
      {
        callocBfProgramData(&data_memory);
      }

      // reset values
      bracket_counter = 0;
      memory_counter = 0;
      loop_counter = 0;

      // load eval instructions and parse
      while (user_input_parameter_two[loop_counter] != '\0')
      {
        next_char = user_input_parameter_two[loop_counter];

        function_error = parseAndSaveCharacter(next_char, &memory_size, 
          &memory_counter, &bracket_counter, &eval_memory);
        loop_counter++;
      }
      if (function_error == 2)
      {
        // out of memory error
        printf(errorOutOfMemory);
      }

      // count correct number of bf loops
      if (bracket_counter != 0)
      {
        printf(errorParsingFailed);
      }
      else
      {
        --memory_counter;

        // create list
        eval_list = createList(memory_counter);
        if (eval_list == NULL)
        {
          // free memory
          freeAllTheMemory(&bf_program, &data_memory, &list, &eval_memory, 
            &eval_list);
          exit(2);
        }

        loadList(eval_list, eval_memory, memory_counter);
        start_node = eval_list;

        // run bf prog
        interpret(&start_node, &data_memory, &data_memory_size,interactive, 
          memory_size ,-1, &shift_right_counter);
        
      }
    }
    else if (strcmp(user_input_parameter_one, "eval") == 0)
    {
      printf(errorWrongParameterCount);
    }

    // break command ----------------------------------------------------------
    if (strcmp(user_input_parameter_one, "break") == 0
      && strcmp(user_input_parameter_two, "default") != 0)
    {
      // check if no program loaded error
      if(bf_program == NULL)
      {
        // no prog loaded error
        printf(errorNoFileLoaded);
      }
      else
      {
        if (checkIfDigits(user_input_parameter_two))
        {
          break_point = atoi(user_input_parameter_two);
          setBreak(list, break_point);
          //printf("Break point set at %c \n", list->character);
        }
        else
        {
          printf(errorWrongUsage);
        }
      }
    }
    else if (strcmp(user_input_parameter_one, "break") == 0)
    {
      printf(errorWrongParameterCount);
    }

    // step command -----------------------------------------------------------
    if (strcmp(user_input_parameter_one, "step") == 0)
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
          step_counter = (unsigned int)atoi(user_input_parameter_two);
        }

        interactive = 0;
        interpret(&start_node, &data_memory, &data_memory_size,interactive,
                  memory_size, step_counter, &shift_right_counter);

      }
    }

    // memory command ---------------------------------------------------------
    if (strcmp(user_input_parameter_one, "memory") == 0)
    {
      // check if no program loaded error
      if(data_memory == NULL)
      {
        // no prog loaded error
        printf(errorNoFileLoaded);
      }
      else
      {
        // default values: number = instruction ptr position; type = hex.
        list_iterator = start_node;
        memory_id = 0; // TODO: set correctly!

        // check for user input
        if (strcmp(user_input_parameter_two, "default") != 0 && 
          strcmp(user_input_parameter_three, "default") != 0)
        {
          // set memory number to ptr head
          memory_id = atoi(user_input_parameter_two);
        }

        if (strcmp(user_input_parameter_three, "int") == 0)
        {
          // print memory in INT
          printf("Integer at %i: %i", memory_id, data_memory[memory_id]);
        }
        else if (strcmp(user_input_parameter_three, "bin") == 0)
        {
          // print memory in BIN
          char binary_value[8];
          inToBinary(data_memory[memory_id], binary_value);
          
          // int to binary function
          printf("Binary at %i: %s", memory_id, binary_value);
        }
        else if (strcmp(user_input_parameter_three, "char") == 0)
        {
          // print memory in CHAR
          printf("Char at %i: %c", memory_id, data_memory[memory_id]);
        }
        else
        {
          // print memory in HEX
          printf("Hex at %i: %x", memory_id, 
            (unsigned int)data_memory[memory_id]);
        }
      
        printf("\n");
      }
    }

    // show command -----------------------------------------------------------
    if (strcmp(user_input_parameter_one, "show") == 0)
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
          show_size_counter = (unsigned int)atoi(user_input_parameter_two);
        }

        list_iterator = start_node;

        // print next SIZE elements OR till EOF
        for (print_counter = 0; print_counter < show_size_counter && 
          list_iterator != NULL; print_counter++)
        {
          printf("%c", list_iterator->character);
          list_iterator = list_iterator->next;
        }

        printf("\n");
      }
    }

    // change command ---------------------------------------------------------
    if (strcmp(user_input_parameter_one, "change") == 0)
    {
      // check if no program loaded error
      if(data_memory == NULL)
      {
        // no prog loaded error
        printf(errorNoFileLoaded);
      }
      else
      {
        change(shift_right_counter, data_memory, change_input);
      }
    }
    else if (strcmp(user_input_parameter_one, "change") == 0 &&
             strcmp(user_input_parameter_two, "default") != 0 &&
             strcmp(user_input_parameter_three, "default") != 0) //------------
    {
      if(checkIfDigits(user_input_parameter_two) &&
              checkIfHex(user_input_parameter_three))
      {
        int data_position = atoi(user_input_parameter_two);
        change_input = convertToDecimal(user_input_parameter_three);
        change(data_position, data_memory, change_input);
      }
      else
      {
        printf(errorWrongUsage);
      }
    }
  }

  // prints quit
  if (strcmp(command, "quit") == 0)
  {
    printf("Bye.\n");
  }

	// free all the memory!
  freeAllTheMemory(&bf_program, &data_memory, &list, &eval_memory, &eval_list);

	return 0;
}