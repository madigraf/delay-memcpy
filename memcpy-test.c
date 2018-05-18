#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "delaymemcpy.h"

#define MAX_ARRAY_SIZE 0x40000000 // 2^30=1GB

//credit to 313 teaching team for initialization and basic testing
//"!!!" marks beginning of more extensive student tests

/* Arrays declared as global, as this allows program to use more
   memory. Aligned to page boundaries so other global variables don't
   end up in same page. */
unsigned char __attribute__ ((aligned (0x1000))) array[MAX_ARRAY_SIZE];
unsigned char __attribute__ ((aligned (0x1000))) copy[MAX_ARRAY_SIZE];

/* Initializes the array with a random set of values. */
void *random_array(void *array, size_t size) {
  
  int i;
  size /= sizeof(long);
  for (i = 0; i < size; i++)
    ((long *)array)[i] = random();
  
  return array;
}

/* Prints all values of the array */
void print_array(unsigned char *array, int size) {
  int i;
  for (i = 0; i < size; i++)
    printf("%02x ", array[i]);
  printf("\n");
}

int main(void) {
  
 //*(int*)0 = 0;  //causes an actual segfault
  srandom(time(NULL));
  
  initialize_delay_memcpy_data();

  
  
  printf("\nCopying one page of data, trigger copy via read dst...\n");
  random_array(array, 0x1000);
  printf("Before copy: ");
  print_array(array, 20);
  
  delay_memcpy(copy, array, 0x1000);
  printf("After copy : ");
  print_array(array, 20); // Should not trigger copy
  printf("Destination: ");
  print_array(copy, 20);  // Triggers copy
  
  printf("\nCopying one page of data, trigger copy via write dst...\n");
  random_array(array, 0x1000);
  printf("Before copy: ");
  print_array(array, 20);
  
  delay_memcpy(copy, array, 0x1000);
  copy[0]++; // Triggers copy
  printf("After copy : ");
  print_array(array, 20);
  printf("Destination: ");
  print_array(copy, 20);
  
  printf("\nCopying one page of data, trigger copy via write src...\n");
  random_array(array, 0x1000);
  printf("Before copy: ");
  print_array(array, 20);
  
  delay_memcpy(copy, array, 0x1000);
  array[0]++; // Triggers copy
  printf("After copy : ");
  print_array(array, 20);
  printf("Destination: ");
  print_array(copy, 20);
  
  printf("\nCopying two pages of data...\n");
  random_array(array, 0x2000);
  printf("Before copy: ");
  print_array(array, 20);
  
  delay_memcpy(copy, array, 0x2000);
  printf("After copy : ");
  print_array(array, 20);
  printf("Destination: ");
  print_array(copy, 20);  // Triggers copy of first page only
  printf("2nd page:\nAfter copy : ");
  print_array(array + 0x1800, 20);
  printf("Destination: ");
  print_array(copy + 0x1800, 20);  // Triggers copy of second page
  
  printf("\nCopying unaligned page of data...\n");
  random_array(array, 0x2000);
  printf("Before copy: ");
  print_array(array + 0x400, 20);
  
  delay_memcpy(copy + 0x400, array + 0x400, 0x1000);
  printf("After copy : ");
  print_array(array + 0x400, 20);
  printf("Destination: ");
  print_array(copy + 0x400, 20);  // Triggers copy of first page only
  
  /* !!! Student tests begin here */

  printf("\nCopying two pages of data...\n");
  random_array(array, 0x2000);
  printf("Before copy: ");
  print_array(array, 20);
  printf("\nBefore copy: ");
  print_array(array + 0x1800, 20);
  
  delay_memcpy(copy, array, 0x2000);
  printf("2nd page:\nAfter copy : ");
  print_array(array + 0x1800, 20);
  printf("Destination: ");
  print_array(copy + 0x1800, 20);  // Triggers copy of second page

  printf("1nd page:\nAfter copy : ");
  print_array(array, 20);
  printf("Destination: ");
  print_array(copy, 20);  // Triggers copy of first page only
  







  
  printf("\nCopying 7 page long piece of data, accessing in the middle first...\n");
  random_array(array, 0x7000);
  printf("Before copy 4th page: ");
  print_array(array + 0x3200, 20);
  printf("Before copy: 1st page:\n");
  print_array(array, 20);


  delay_memcpy(copy, array, 0x7000);
  printf("After copy : 4th page ");
  print_array(array + 0x3200, 20);
  printf("Destination: 4th page ");
  print_array(copy + 0x3200, 20);  // Triggers copy of fifth page only


  printf("After copy : 1st page ");
  print_array(array, 20);
  printf("Destination: 1st page ");
  print_array(copy, 20);  // Triggers copy of first page only

  printf("After copy : 3rd page ");
  print_array(array + 0x2700, 20);
  printf("Destination: 3rd page ");
  print_array(copy + 0x2700, 20);  // Triggers copy of third page only

  printf("After copy : 7th page ");
  print_array(array + 0x3200, 20);
  printf("Destination: 7th page ");
  print_array(copy + 0x3200, 20);  // Triggers copy of 7th page only

  printf("After copy : 6th page ");
  print_array(array + 0x5600, 20);
  printf("Destination: 6th page ");
  print_array(copy + 0x5600, 20);  // Triggers copy of 7th page only

  printf("After copy : 5th page ");
  print_array(array + 0x4900, 20);
  printf("Destination: 5th page ");
  print_array(copy + 0x4900, 20);  // Triggers copy of 7th page only

  printf("After copy : 2nd page ");
  print_array(array + 0x1040, 20);
  printf("Destination: 2nd page ");
  print_array(copy + 0x1040, 20);  // Triggers copy of 7th page only


  printf("Range tests \n");
  
  random_array(array, 0x8000);
  printf("Before copy2 page: ");
  print_array(array + 0x3300, 20);

  printf("Before copy1 2nd page: ");
  print_array(array + 0x1300, 20);
  
  printf("Before copy3 page 1: ");
  print_array(array + 0x6400, 20);
  printf("Before copy3 page 2: ");
  print_array(array + 0x7800, 20);

  printf("Before copy1 1st page: ");
  print_array(array, 20);

  delay_memcpy(copy, array, 0x2000);
  delay_memcpy(copy + 0x3000, array + 0x3000, 0x1000);
  delay_memcpy(copy + 0x6000, array + 0x6000, 0x2000);


  printf("After copy2 page:  ");
  print_array(array + 0x3300, 20);
  printf("Destination: copy2 page ");
  print_array(copy + 0x3300, 20);  // Triggers copy of first page only

  printf("After copy1 2nd page ");
  print_array(array + 0x1300, 20);
  printf("Destination: copy1 2nd page ");
  print_array(copy + 0x1300, 20);  // Triggers copy of third page only

  printf("After copy : copy3 page1 ");
  print_array(array + 0x6400, 20);
  printf("Destination: copy3 page1 ");
  print_array(copy + 0x6400, 20);  // Triggers copy of 7th page only

  printf("After copy : copy3 page 2:  ");
  print_array(array + 0x7800, 20);
  printf("Destination: copy3 page 2: ");
  print_array(copy + 0x7800, 20);  // Triggers copy of 7th page only

  printf("After copy : copy1 1st page:");
  print_array(array, 20);
  printf("Destination: copy1 1st page:");
  print_array(copy, 20);  // Triggers copy of 7th page only

 

  printf("Dependency tests \n");

  printf("Casual Dependency Test \n");

  random_array(array, 0x8000);
  printf("Before A: ");
  print_array(array, 20);
  printf("Before B: ");
  print_array(copy, 20);
  printf("Before C: ");
  print_array(copy + 0x2000, 20);

  delay_memcpy(copy, array, 0x1000);
  delay_memcpy(copy + 0x2000, copy, 0x1000);

  
  printf("After C: ");
  print_array(copy + 0x2000, 20);  
  printf("After A:");
  print_array(array, 20);
  printf("After B:");
  print_array(copy, 20);

  printf("Output Dependency Test \n");

  random_array(array, 0x8000);
  memcpy(copy, array + 4000, 0x1000);
  printf("Before A: ");
  print_array(array, 20);
  printf("Before B: ");
  print_array(copy, 20);
  printf("Before C: ");
  print_array(copy + 0x2000, 20);

  delay_memcpy(copy, array, 0x1000);
  delay_memcpy(copy, copy + 0x2000, 0x1000);

  
  printf("After C: ");
  print_array(copy + 0x2000, 20); 
  printf("After B:");
  print_array(copy, 20);
  printf("After A:");
  print_array(array, 20);
  

  printf("Anti-Dependency Test \n");

  random_array(array, 0x8000);
  memcpy(copy, array + 4000, 0x1000);
  printf("Before C: ");
  print_array(copy + 0x2000, 20);
  printf("Before A: ");
  print_array(array, 20);
  printf("Before B: ");
  print_array(copy, 20);
  
  delay_memcpy(copy, array, 0x1000);
  delay_memcpy(array, copy + 0x2000, 0x1000);

  
  printf("After C: ");
  print_array(copy + 0x2000, 20); 
  printf("After A:");
  print_array(array, 20);
  printf("After B:");
  print_array(copy, 20);

  //bonus code tests
  printf("\nCopying unaligned page of data with different offsets...\n");
  random_array(array, 0x2000);
  printf("Before copy: ");
  print_array(array + 0x400, 20);
  print_array(copy + 0x800, 20);
  
  delay_memcpy(copy + 0x800, array + 0x400, 0x1000);
  printf("After copy : ");
  print_array(array + 0x400, 20);
  printf("Destination: ");
  print_array(copy + 0x800, 20);  // Triggers copy of first page only*/
  


  
  
  return 0;
}
