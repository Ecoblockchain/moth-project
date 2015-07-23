#include <stdio.h>
#include <unistd.h>

int main() {
  int seconds = 5;
  while (seconds > 0) {
    printf("Starting logger in %i seconds (press any key to skip)\r", seconds);
    seconds--;
  }
  printf("\n");
}
