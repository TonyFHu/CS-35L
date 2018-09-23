#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void reportError(const char* msg)
{
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

void checkIOError(FILE* file)
{
  if (ferror(file))
    reportError("IO Error");
}

int checkInput(int argc, char*** argv)
{
  int i, j;
  char** args = *argv;
  //check for # args
  if (argc != 3)
    reportError("Invalid number of arguments; Expected 2");
  //check for same length
  if (strlen(args[1]) != strlen(args[2]))
    reportError("Argument lengths do not match");
  //check for duplicate chars
  for (i = 0; i < strlen(args[1]); i++)
    {
      for (j = 0; j < strlen(args[1]); j++)
	{
	  if (i != j)
	    {
	      if ((args[1][i] == args[1][j]) || (args[2][i] == args[2][j]))
		reportError("Duplicate char in argument strings");
	    }
	}
    }
}

char translate(char c, char** d_in, char** d_out)
{
  char* in, * out;
  int i;
  in = *d_in;
  out = *d_out;
  for (i = 0; i < strlen(in); i++)
    {
      if (in[i] == c)
	return out[i];
    }
  return c;
}

int main(int argc, char** argv)
{
  char* in, * out, c;
  checkInput(argc, &argv);
  in = argv[1];
  out = argv[2];
  for (;;)
    {
      c = getchar();
      checkIOError(stdin);
      if (feof(stdin))
	break;
      putchar(translate(c, &in, &out));
      checkIOError(stdin);
    }
  return 0;
}
