Build
=====    

Examples:
```
cmake -DBUILD_EXAMPLES . && ./out/examples/example-*
```    
    
Tests:
```
cmake -DBUILD_TESTING=ON . && ctest -V
```    

TODO
====    
 - set static buffer size based on real usage data
 - add more Buffer::put overloads
 - add the buffer expansion ability
 - profile methods
 - log parent method and file names
 - make sure that no strings were added to result app in release mode