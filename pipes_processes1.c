// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{  
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int k,i;
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
    char concat_str[100]; 
    char fixed_str[] = "howard.edu"; 
    char input_str[100];
    pid_t p;
    
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
  
        close(fd1[0]);  // Close reading end of pipes 
        close(fd2[1]);

        printf("Enter a string to concatenate:");
        scanf("%s", input_str); 
  
        // Write input string and close writing end of first 
        // pipe. 
        write(fd1[1], input_str, strlen(input_str)+1); 
        
        read(fd2[0], concat_str, 100); 
       
        printf("%s\n",concat_str);
        
    } 
  
    // child process 
    else
    { 
        close(fd1[1]);  // Close writing end of first pipes 
        close(fd2[0]); 
      
        // Read a string using first pipe 
        
        read(fd1[0], concat_str, 100); 
  
        // Concatenate a fixed string with it 
        k = strlen(concat_str); 
     
        for (i=0; i<strlen(fixed_str); i++) 
            concat_str[k++] = fixed_str[i]; 
  
        concat_str[k] = '\0';   // string ends with '\0' 
  
        printf("Enter a string to concatenate:");
        scanf("%s", input_str); 
  
        // Concatenate a fixed string with it 
        k = strlen(concat_str); 
        for (i=0; i<strlen(input_str); i++) 
            concat_str[k++] = input_str[i]; 
  
        concat_str[k] = '\0';   // string ends with '\0' 

        // Write input string and close writing end of first 
        // pipe. 
        write(fd2[1], concat_str, strlen(concat_str)+1); 

        

      
    } 
} 