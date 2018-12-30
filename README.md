##**REGULAR EXPRESSION LIBRARY**

**NAME:**  libgen.a

**USAGE:**

  Insert the line below in order to use the functions defined in the library
  
    #include <regexpr.h>
    
  Next compile your program as follows:
  
    cc your-program.c -lgen  
  
  Matching is achived in two steps:
  1.   Compile the regexp into internal program representation of it by using compile,
  
        char * cp = compile("abc. *$", 0, 0);
         
        cp becomes the compiled version of the regexp we want to match
        
  2.    Actually, find the match:
  
        char *loc1, *loc2;
  
        int ret = step("sffdhabc12345", cp);
        
        if ret = 1 we succeed to find a match
        
        and loc1 points the start point of the match, while loc2 points one past the end of the match.
        
   3. Have a look for more examples in the test directory.
        
