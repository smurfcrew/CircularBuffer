# Circular Buffer

## Note
The rBuffer.h file is/was the only file that I worked on, and the main.cpp was provided as this was a project for a data structures class. This project was to implement a circular buffer using templates and dynamic memory allocation.

## Description 
In this program we are going to implement a ring buffer or circular buffer. The specialty of this kind of buffer is that it loops over itself creating a circular or ring structure. When the last location in this buffer is filled up, it loops around and starts over from the beginning. This type of buffer is commonly used for streaming.

Using dynamic memory allocation to implement this kind with 3 major pointers. The first pointer buffer_p holds the dynamically allocated buffer. The second pointer write_p starts at the first index of the buffer and increments itself after writing data. The third pointer read_p also starts at the first index of the buffer, reads something, and then increments itself. The read pointer cannot read past the write pointer because if it does reach the write pointer, it has already read everything prior to it. Both these pointers loop around to the beginning of the buffer after they pass the end of it. Technically, this type of buffer does not have a beginning or an end.

## Output Validation
A way to check if the output is correct is to run the program and compare the output to the sample output.
Do not modify the main function and expect it to match the sample output. It will be different. 

## To run
        
```bash
$ make
$ ./main 
$ ./main > test_output.txt
# To check if it runs properly you can output it to a file then compare it to the sample output.
$ diff test_output.txt sample_output.txt
# To check for memory leaks. 
$ valgrind --tool=memcheck --leak-check=yes --show-reachable=yes -–show—leak-kinds=all --track-origins=yes --track-fds=yes -s ./main
```
