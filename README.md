#### Summary
 * This project includes c++ header files and libraries to execute tasks simultaneously.
 * Defined tasks are registered into Queue, and those tasks are executed step by step.
 * The Cmake files have hierarchy according to folder structure. So, you have to first run "cmake CMakeLists.txt" in the root folder, and then execute "make" in the /src/subdirectory.
 * Unit test is based on Google-test.
 * You can refer README.md in the /src/subdirectory for detail information of each subproject.

---
#### Directory Structure
 * bin : binary files generated after build
 * example : You can find out some examples that are based on gtaskque and other libraries.
 * include : composed of sub-project directories that contain header files. You can include header files, #include "sub-directory/*.h", because the "include" directory is referred as include_directories() in a Cmake file. Some header files can be used independantly and some should be used with library files located in the "lib" directory.
 * lib : static or dynamic library files installed by "make install". You have to add the line to CMakeLists.txt in the ./src/*lib as "install(TARGETS samplelib DESTINATION ${LIBDIR})"
 * src : composed of sub-project directories that contain source codes, *.h, *.cpp. 
 * test : composed of test directories for unit test.

```
ezeit
├── bin
├── example
├── include
    └── example
    └── examplelib
    └── gexecutor
    └── glogger    
    └── gtaskque
        └── gtaskque.h
    └── ubyte   
├── lib
├── src
    └── example
    └── examplelib
    └── gexecutor
    └── glogger    
    └── gtaskque
        └── gtaskque.h
        └── README.md
    └── ubyte    
├── test
    └── unittest
├── CMakeLists.txt
└── README.md
```



