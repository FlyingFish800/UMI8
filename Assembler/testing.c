#include <stdio.h>
#include <stdlib.h>

void push(int *i){
    *i+=1;
}

int main(){
    int *i = malloc(8);
    printf("%p:%d\n",i,*i);
    push(i);
    printf("%p:%d\n",i,*i);
    return 0;
}

int main1(){
    int *i = malloc(0);
    printf("%p\n",i);
    for (int x = 0; x < 0xFFFFFFFF; x++){
    i = realloc(i,x);
    printf("%lu\n",sizeof(i));}
    return 0;
}

#include <stdio.h>
#include <stdlib.h>

int main2() {
  int n;
  printf("Size of the array: ");
  scanf("%d", &n);
  int *arr = calloc(n, sizeof(int)); // Creating enough space for 'n' integers.
  if (arr == NULL) {
    printf("Unable to allocate memory\n");
    return -1;
  }
  printf("Enter the elements (space/newline separated): ");
  for (int i = 0; i < n; i++)
    scanf("%d", arr + i); // Notice that, (arr + i) points to ith element

  printf("Given array: ");
  for (int i = 0; i < n; i++)
    printf("%d ", *(arr + i)); // Dereferencing the pointer
  printf("\n");

  printf("Removing first element i.e., arr[0] = %d.\n", arr[0]);
  for (int i = 1; i < n; i++)
    arr[i - 1] = arr[i];
  arr = realloc(arr, (n - 1) * sizeof(int));

  printf("Modified Array: ");
  for (int i = 0; i < n - 1; i++)
    printf("%d ", arr[i]);
  printf("\n");

  free(arr);
  return 0;
}