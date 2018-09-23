#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>


int ignoreCase = 0;

void print(char* msg)
{
  write(2, msg, strlen(msg));
}

int frobcmp(const char* a, const char* b)
{
  int i;
  for (i = 0; a[i] == b[i]; i++)
    {
      if (a[i] == ' ' || b[i] == ' ')
	return 0;
    }
  if (!ignoreCase)
    { 
      if ((a[i]^42) < (b[i]^42))
	return -1;
      else if ((a[i]^42) > (b[i]^42))
	return 1;
    }
  else
    {
      if ((toupper((uint)a[i]^42)) < (toupper((uint)b[i]^42)))
	return -1;
      else if ((toupper((uint)a[i]^42)) > (toupper((uint)b[i]^42)))
	return 1;
    }
  return 0;
}

int wrapper(const void* a, const void* b)
{
  return frobcmp( *((const char**) a), *((const char**) b));
}

int checkIOError(int a)
{
  if ( a < 0 )
    {
      print("IO Error");
      return 1;
    }
  return 0;
}

int checkMemError(void* pointer)
{
  if (pointer == NULL)
    {
      print("Memory Error");
      return 1;
    }
  return 0;
}

int checkOptions(int argc, char** argv)
{
  if (argc > 2)
    {
      print("Unexpected arguments");
      exit(1);
    }
  if (argc == 2)
    {
      if (strcmp(argv[1], "-f") == 0)
	return 1;
      print("Unknown argument");
      exit(1);
    }
  return 0;
}

int main(int argc, char** argv)
{
  int (* compare) (const void*, const void*);
  char c;
  char* input;
  char* temp;
  char** lines;
  char** copy;
  int i, j, k, memory, initSize, numChars, numLines, currentLine, l;
  struct stat fileinfo;

  ignoreCase = checkOptions(argc, argv);
  
  l = fstat(0, &fileinfo);
  if (l < 0)
    print("read failure");

  initSize = fileinfo.st_size+3;
  memory = initSize;
  numChars = 0;
  numLines = 0;
  currentLine = 0;
  
  input = (char*) malloc(memory);
  if (checkMemError(input))
      exit(1);
  
  while (l = read(0, &c, 1))
    {
      if (checkIOError(l))
	{
	  free(input);
	  exit(1);
	}
   
      input[numChars] = c;
      currentLine++;
      if (input[numChars] == ' ')
	{
	  numLines++;
	  currentLine = 0;
    	}
      numChars++;
      if (numChars >= memory-2)
	{
	  memory *= 2;
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
	  c = *(copy[i]);
	  l = write(1, &c, 1);
	  if (checkIOError(l))
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

  return 0;
}

