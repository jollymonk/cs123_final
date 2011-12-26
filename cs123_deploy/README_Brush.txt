
Molly Junck (mjunck)
CS123 Fall 2011

Brush

My design is pretty straight forward, I added a few helper methods in the Brush class to pull out common functionality, but mostly followed support code design.  The constant, linear and quadratic brushes all use the Brush class paintOnce() method, and the smudge brush overwrites it to implement smudging functionality.  

I ran valgrind and it appeared that all the memory leaks were from qtcreator.  I also ran into the issue (purely C++ unfamiliarity) that I couldn?t initialize pointers to NULL in the header file as an added safety, instead I got an ?ISO C++ forbids the initalizaiton of member <member_name>? error.  I like safety.  

:0)