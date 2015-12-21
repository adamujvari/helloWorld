//-----------------------------------------------------------------------------
// assa.c
//
// brainfuck interpreter
// 
// This program is a brainfuck interpreter with debug functionality.
// The user can run the program in two different modes, interpret only and
// debug. 
//
// Group: 2 - study assistant Angela Promitzer
//
// Authors: Adam Ujvari     1232056
// 			    Amel Hamidovic  1330013
//			    Tina Promitzer  1311885
//
// Latest Changes: 21.12.2015
//-----------------------------------------------------------------------------
//

// libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// defines
#define BUFFER_SIZE 1024
#define errorWrongUsage "[ERR] usage: ./assa [-e brainfuck_filnename]\n"
#define errorReadingFile "[ERR] reading the file failed\n"
#define errorWrongParameterCount "[ERR] wrong parameter count\n"
#define errorNoFileLoaded "[ERR] no program loaded\n"
#define errorOutOfMemory "[ERR] out of memory\n"
#define errorParsingFailed "[ERR] parsing of input failed\n"

// typedef struct
typedef struct _Node_
{
  char character_;
  char is_break_;
  int position_;
  struct _Node_ *next_;
  struct _Node_ *begin_;
  struct _Node_ *end_;
} Node;

//-----------------------------------------------------------------------------
///
/// Free linked list.
///
/// @param Node *list ptr to list to be free'd.
///
/// @return none.
//
void freeList(Node *list)
{
  Node *current_pos = list;
  Node *next_pos;

  while (current_pos != NULL)
  {
    next_pos = current_pos->next_;
    free(current_pos);
    current_pos = next_pos;
  }
}

//-----------------------------------------------------------------------------
///
/// Create linked list for brainfuck instructions.
///
/// @param int num_of_elements number of bf instructions.
///
/// @return Node *ptr head if successful.
/// @return NULL ptr if out of memory.
//
Node *createList(int num_of_elements)
{
  int counter;
  Node *head = malloc(sizeof(Node));
  if (head == NULL)
  {
    printf(errorOutOfMemory);
    return NULL;
  }

  head->character_ = 0;
  head->is_break_ = 0;
  head->position_ = 0;
  head->next_ = NULL;
  head->begin_ = NULL;
  head->end_ = NULL;

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

    new->character_ = 0;
    new->is_break_ = 0;
    new->position_ = counter;
    new->next_ = NULL;
    new->begin_ = NULL;
    new->end_ = NULL;

    current_pos->next_ = new;
    current_pos = new;
  }

  return head;
}

//-----------------------------------------------------------------------------
///
/// Takes the parsed brainfuck instructions and loads them into the already 
/// created linked list elements.
///
/// @param Node *list ptr to head of linked list.
/// @param unsigned char *bf_program ptr to parsed instructions.
/// @param int memory_counter number of bf instructions.
///
/// @return none.
//
void loadList(Node *list, unsigned char *bf_program, int memory_counter)
{
  int counter;
  int bracket_counter = 0;
  Node *brackets[memory_counter/2];

  for (counter = 0; counter <= memory_counter; counter++)
  {
    list->character_ = bf_program[counter];
    if (bf_program[counter] == '[')
    {
      // save position of loop start
      brackets[bracket_counter++] = list;
    }
    else if (bf_program[counter] == ']')
    {
      // loop start position
      list->begin_ = brackets[--bracket_counter];

      // loop start points to loop end
      list->begin_->end_ = list;
    }

    list = list->next_;
  }
}

//-----------------------------------------------------------------------------
///
/// Takes a character and checks it for correctnes. Fills it into the specified
/// array of instructions.
///
/// @param char character to be checked.
/// @param unsigned int *memory_size ptr to size of allocated memory of array.
/// @param int *memory_counter ptr to number of elements in array.
/// @param int *bracket_counter ptr to number of brackets.
/// @param unsigned char **bf_program double ptr to bf instructions.
///
/// @return int 0 if successful.
/// @return int 2 if out of memory.
//
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

//-----------------------------------------------------------------------------
///
/// Opens a specified file, if said file is present and a correct it parses its
/// contents and fills the instructions character by character into a linked 
/// list.
///
/// @param unsigned char **bf_program double ptr to original instructions.
/// @param char *bf_prog_name ptr to the file name to open.
/// @param unsigned int *memory_size ptr to current allocated memory size.
/// @param int *memory_counter ptr to number of bf instructions.
///
/// @return int 0 if successful.
/// @return int 1 if reading the file failed.
/// @return int 2 if out of memory.
//
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
    while ((next_char = (char)fgetc(bf_file_ptr)) != EOF)
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

//-----------------------------------------------------------------------------
///
/// Takes a double pointer and allocates the defined size of memory for the bf
/// instruction memory and zero initializes it.
///
/// @param unsigned char **bf_program_data double ptr to allocate memory for
///
/// @return none.
//
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

//-----------------------------------------------------------------------------
///
/// Allocates new memory for data memory.
///
/// @param unsigned char *data_memory ptr to the data memory.
/// @param int old_size current size of the data memory.
/// @param int new_size new, doubled size of the data memory.
///
/// @return unsigned char *ptr of new data memory if successful.
/// @return NULL ptr if out of memory.
//
unsigned char *callocData(unsigned char *data_memory, int old_size,
  int new_size)
{
  unsigned char *new_memory = calloc(sizeof(unsigned char), (size_t)new_size);
  if(new_memory == NULL)
  {
    return NULL;
  }

  // copy contents into new array
  memcpy(new_memory, data_memory, (size_t)old_size);
  free(data_memory);
  return new_memory;
}

//-----------------------------------------------------------------------------
///
/// Checks if user input were digits only.
///
/// @param char *user_input_parameter ptr to user input.
///
/// @return int 0, false if other symbols found.
/// @return int 1, true if contains only digits.
//
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

//-----------------------------------------------------------------------------
///
/// Checks if user input was a correct hex input.
///
/// @param char *user_input_parameter ptr to user input.
///
/// @return int 0, false if not correct.
/// @return int 1, true if correct.
//
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

//-----------------------------------------------------------------------------
///
/// Converts user input to a decimal number.
///
/// @param char *user_input_parameter ptr to user input.
///
/// @return unsigned char decimal_number converted number.
//
unsigned char convertToDecimal(char *user_input_parameter)
{
  const int base = 16;
  long long_number = 0;
  unsigned char decimal_number;

  long_number  = strtol(user_input_parameter, NULL, base);

  if(long_number < 256 && long_number > 0)
  {
    decimal_number = (unsigned char) long_number;
  }
  else
  {
    return 0;
  }

  return decimal_number;
}

//-----------------------------------------------------------------------------
///
/// Changes the value on the given position to user input in the data memory.
///
/// @param int position to be changed.
/// @param unsigned char *data_memory ptr to data memory.
/// @param unsigned char input to be changed to.
///
/// @return none.
//
void change(unsigned char *data_memory, int position, unsigned char input)
{
  data_memory[position] = input;
}

//-----------------------------------------------------------------------------
///
/// Interprets the brainfuck instructions
///
/// @param Node **start_node double ptr to instruction to be interpreted.
/// @param unsigned char *data_memory ptr to the head of the data memory.
/// @param unsigned char **data_memory_position double ptr to current position
///        in the data memory.
/// @param int *data_memory_size ptr to the size of data memory.
/// @param int *step_right_counter ptr to number of right steps in the data
///        memory.
/// @param int command_count overall number of bf instructions.
/// @param int step_counter number of instructions currently interpreted.
/// @param int interactive bool that switches interpreting between interactive
///        and simple mode.
///
/// @return none.
//
void interpret(Node **start_node, unsigned char *data_memory,
  int *data_memory_size, unsigned char **data_memory_position,
    int *step_right_counter, int command_count, int step_counter,
      int interactive)
{
  unsigned char *temp_memory; // temporary memory for reallocing data memory
  unsigned int go_right_counter = 0; // counter to check if realloc is needed
  Node *temp_next; //temporary next node from the program list

  // check if no more bf instructions
  if((*start_node) != NULL)
  {
    if((((*start_node)->position_) + step_counter) > command_count)
    {
      step_counter = command_count;
    }
  }
  else
  {
    printf(errorNoFileLoaded);
    return;
  }

  while ((*start_node) != NULL)
  {
    // break loop if # of steps done
    if(step_counter == 0)
    {
      return;
    }
    else
    {
      // check if data_memory needs to expanded
      if ((go_right_counter + 1 % BUFFER_SIZE) == 0)
      {
        temp_memory = callocData(*data_memory_position, *data_memory_size,
          (*data_memory_size) * 2);

        // set new size of data memory
        *data_memory_size *= 2;

        if (temp_memory == NULL)
        {
          printf(errorOutOfMemory);
          free(*data_memory_position);
          return;
        }
        else 
        {
          *data_memory_position = temp_memory;
          free(temp_memory);
        }
      }

      temp_next = (*start_node)->next_;

      // checks if current node is a breakpoint
      if (interactive == 1 && (*start_node)->is_break_ == 1)
      {
        (*start_node)->is_break_ = 0;
        return;
      }
      else
      {
        switch ((*start_node)->character_)
        {
          case ('>'):
            go_right_counter++;
            (*step_right_counter)++;
            (*data_memory_position)++;
            break;
          case ('<'):
            if ((*data_memory_position) != data_memory)
            {
              (*step_right_counter)--;
              (*data_memory_position)--;
            }
            break;
          case ('+'):
            (**data_memory_position)++;
            break;
          case ('-'):
            (**data_memory_position)--;
            break;
          case ('.'):
            putchar(**data_memory_position);
            break;
          case (','):
            **data_memory_position = (unsigned char)getchar();
            break;
          case ('['):
            if (**data_memory_position == 0)
            {
              temp_next = (*start_node)->end_->next_;
            }
            break;
          case (']'):
            if (**data_memory_position != 0)
            {
              temp_next = (*start_node)->begin_;
            }
            break;
          default:
            printf("Shit is on fire!\n");
        }
      }

      (*start_node) = temp_next;
      step_counter--;
    }
  }
}

//-----------------------------------------------------------------------------
///
/// Sets a break point at the given position.
///
/// @param Node *list ptr to head of bf instruction linked list.
/// @param int breakpoint id of instruction where breakpoint has to be set.
///
/// @return none.
//
void setBreak(Node *list, int breakpoint)
{
  while(list != NULL)
  {
    if(list->position_ == breakpoint)
    {
      list->is_break_ = 1;
      return;
    }
    list = list->next_;
  }
}

//-----------------------------------------------------------------------------
///
/// Frees all allocated memory.
///
/// @param unsigned char **bf_program double ptr to bf instruction memory
/// @param unsigned char **data_memory double ptr to data memory
/// @param Node **list double ptr to bf instruction linked list
/// @param unsigned char **eval_memory double ptr to eval instruction memory.
/// @param Node **eval_list double ptr to eval instruction linked list.
///
/// @return none.
//
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

//-----------------------------------------------------------------------------
///
/// Converts an integer to binary.
///
/// @param unsigned int number to be converted.
/// @param char *binary_value ptr to converted output.
///
/// @return none.
//
void intToBinary(unsigned int number, char *binary_value)
{
  int loop_counter;
  binary_value[8] = '\0';

  for (loop_counter = 7; loop_counter >= 0; --loop_counter, number >>= 1)
  {
      binary_value[loop_counter] = (number & 1) + '0';
  }

}

//-----------------------------------------------------------------------------
///
/// The main program. 
/// Reads and interprets the esoteric porgramming language: Brainfuck. It also
/// has debug options.
/// Switches between program run modes.
/// Handles user input, commands and errors. 
///
/// @param int argc the number of parameters entered at program start.
/// @param const char *argv[] ptr to the parameters entered at program start.
///
/// @return int 0 if program exits without any errors.
/// @return int 1 if wrong usage of program.
/// @return int 2 if out of memory.
/// @return int 3 if parsing the file in normal mode failed.
/// @return int 4 if reading the file in normal mode failed.
//
int main(int argc, const char *argv[])
{
  Node *list = NULL;
  Node *eval_list = NULL;
  Node *eval_list_head = NULL;
  Node *list_iterator = NULL;
  Node *start_node = NULL;
  
  char command[BUFFER_SIZE] = "default";
  char bf_file_name[128];
  char *command_splits = "default";
  char user_input_parameter_one[BUFFER_SIZE];
  char user_input_parameter_two[BUFFER_SIZE];
  char user_input_parameter_three[BUFFER_SIZE];
  char next_char;

  unsigned char change_input = 0;
  unsigned char *bf_program = NULL;
  unsigned char *eval_memory = NULL;
  unsigned char *data_memory = NULL;
  unsigned char *data_memory_position = NULL;

  int data_memory_size = BUFFER_SIZE;
  int print_counter = 0;
  int function_error = 0;
  int memory_counter = 0;
  int interactive = 0;
  int step_right_counter = 0;
  int break_point = 0;
  int bracket_counter = 0;
  int memory_id = 0;
  int loop_counter = 0;

  unsigned int memory_size = BUFFER_SIZE;
  unsigned int show_size_counter = 0;
  unsigned int step_counter = 0;


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
      function_error = loadBfProgram(&bf_program, bf_file_name, &memory_size,
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
      data_memory_position = data_memory;

      interpret(&start_node, data_memory, &data_memory_size,
        &data_memory_position, &step_right_counter, memory_size, -1,
          interactive);

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

    // reset input parameters to default
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
      
      // reset memory & values
      callocBfProgramData(&bf_program);
      callocBfProgramData(&data_memory);
      memory_size = BUFFER_SIZE;
      step_right_counter = 0;

      // load program and parse
      function_error = loadBfProgram(&bf_program, user_input_parameter_two,
        &memory_size, &memory_counter);
      if (function_error == 1)
      {
        // error in readin file: free and reset values
        freeAllTheMemory(&bf_program, &data_memory, &list, &eval_memory,
           &eval_list);
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
        data_memory_position = data_memory;
      }
    }
    else if (strcmp(user_input_parameter_one, "load") == 0)
    {
      printf(errorWrongParameterCount);
    }

    // run command ------------------------------------------------------------
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
        interpret(&start_node, data_memory, &data_memory_size,
          &data_memory_position, &step_right_counter, memory_size, -1,
            interactive);
      }
    }

    // eval command -----------------------------------------------------------
    if (strcmp(user_input_parameter_one, "eval") == 0
      && strcmp(user_input_parameter_two, "default") != 0)
    {
      // check entered bf str length
      if (strlen(user_input_parameter_two) < 81)
      {
        // allocate eval instruction memory
        callocBfProgramData(&eval_memory);

        // check whether program data exists, if no create.
        if (data_memory == NULL)
        {
          callocBfProgramData(&data_memory);
        }

        // reset values
        memory_size = BUFFER_SIZE;
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
          eval_list_head = eval_list;

          if (data_memory_position == NULL)
          {
            data_memory_position = data_memory;
          }

          // run bf prog
          interpret(&eval_list, data_memory, &data_memory_size,
            &data_memory_position, &step_right_counter, memory_size, -1,
              interactive);
        }

        // free eval instructions
        freeList(eval_list_head);
        eval_list_head = NULL;

        free(eval_memory);
        eval_memory = NULL;
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
          if (checkIfDigits(user_input_parameter_two))
          {
            step_counter = (unsigned int)atoi(user_input_parameter_two);
          }
        }

        interpret(&start_node, data_memory, &data_memory_size,
          &data_memory_position, &step_right_counter, memory_size,
            step_counter, interactive);
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
        memory_id = step_right_counter;

        // check for user input
        if (strcmp(user_input_parameter_two, "default") != 0)
        {
          // set memory number to ptr head
          if (checkIfDigits(user_input_parameter_two))
          {
            memory_id = atoi(user_input_parameter_two);
          }
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
          intToBinary(data_memory[memory_id], binary_value);

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
        if (strcmp(user_input_parameter_two, "default") != 0 && 
          checkIfDigits(user_input_parameter_two))
        {
          show_size_counter = (unsigned int)atoi(user_input_parameter_two);
        }

        list_iterator = start_node;

        // print next SIZE elements OR till EOF
        for (print_counter = 0; print_counter < show_size_counter && 
          list_iterator != NULL; print_counter++)
        {
          printf("%c", list_iterator->character_);
          list_iterator = list_iterator->next_;
        }

        printf("\n"); // CHECK: no '\n' if end of instructions
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
        // set defaults
        memory_id = step_right_counter;
        change_input = 0;

        // set memory position if present
        if (strcmp(user_input_parameter_two, "default") != 0)
        {
          if (checkIfDigits(user_input_parameter_two))
          {
            // if digit set position
            memory_id = atoi(user_input_parameter_two);
          }
        }

        // set input if present
        if (strcmp(user_input_parameter_three, "default") != 0)
        {
          if (checkIfHex(user_input_parameter_three))
          {
            change_input = convertToDecimal(user_input_parameter_three);
          }
        }

        change(data_memory, memory_id, change_input); 
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