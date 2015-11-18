#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[] ){
  int value;
  value = atoi(argv[1]);
  if ((value > -1) && (value < 256)){
    return value;
  }
  puts("Incorrect input! Use int from 0 up to 255");
  return 1;
}
