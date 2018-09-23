#include <stdio.h>
#include <stdlib.h>


int frobcmp(const char* a, const char* b)
{
  int i;
  for (i = 0; a[i] == b[i]; i++)
    {
      if (a[i] == ' ' || b[i] == ' ')
	return 0;
    }
  if ((a[i]^42) < (b[i]^42))
    return -1;
  else if ((a[i]^42) > (b[i]^42))
    return 1;
  return 0;
}

void kill(char a)
{
  fprintf(stderr, "%c\n", a);
}

int wrapper(const void* a, const void* b)
{
  return frobcmp( *((const char**) a), *((const char**) b) );
}

int checkIOError(FILE* pointer)
{
  if ( ferror(pointer) )
    {
      fprintf(stderr, "IO Error");
      return 1;
    }
  return 0;
}

int checkMemError(void* pointer)
{
  if (pointer == NULL)
    {
      fprintf(stderr, "Memory Error");
      return 1;
    }
  return 0;
}

int main()
{
  int (* compare) (const void*, const void*);
  char c;
  char* input;
  char* temp;
  char** lines;
  char** copy;
  int i, j, k, memory, initSize, numChars, numLines, currentLine;
  initSize = 10;
  memory = initSize;
  numChars = 0;
  numLines = 0;
  currentLine = 0;
  
  input = (char*) malloc(memory);
  if (checkMemError(input))
      exit(1);
  
  while (!feof(stdin))
    {
      c = getchar();
      if (checkIOError(stdin))
	{
	  free(input);
	  exit(1);
	}
      if (feof(stdin) || c == EOF)
	break;
      if ( c == ' ' && currentLine == 0 ) //skip leading spaces
     	continue;
      input[numChars] = c;
      currentLine++;
      if (input[numChars] == ' ')
	{
	  numLines++;
	  currentLine = 0;
    	}
      numChars++;
      if (numChars == memory-2)
	{
	  memory += 10;
	  temp = (char*) realloc(input, memory);
	  if (checkMemError(temp))
	    {
	      free(input);
	      exit(1);
	    }
	  input = temp;
	}
    }
  if (input[numChars-1] != ' ')
    {
      input[numChars] = ' ';
      numLines++;
      numChars++;
    }
  lines = (char**) malloc(sizeof(char*)*numLines);
  if (checkMemError(lines))
    {
      free(input);
      exit(1);
    }

  temp = input;
  j = 0;
  for(i = 0; i < numChars; i++)
    {
      if (input[i] == ' ')
	{
	  lines[j] = temp;
	  j++;
	  temp = input + i + 1; //temp now points to after the space in the array 
	}
    } 
 
  compare = &wrapper;
  qsort(lines, numLines, sizeof(char*), compare);
  copy = lines;
 
  for( i = 0; i < numLines; i++)
    {
      for (;;)
	{
	  putchar(*(copy[i]));
	  if (checkIOError(stdout))
	    {
	      free(input);
	      free(lines);
	      exit(1);
	    }
	  if (*(copy[i]) == ' ')
	    break;
	  copy[i]++;
	}
    }

  free(lines);
  free(input);
  return 0;
}

