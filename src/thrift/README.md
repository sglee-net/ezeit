# Guideline

## Run thrift
thrift -r --gen cpp messenger.thrift
thrift -r --gen java messenger.thrift
thrift -r --gen py messenger.thrift

## How to make and use .thrift files
### Case of C++
You have to modify the funtion of operator== in ThriftRWService.h 
### Server - Client
Client: "required" is needed for parameter you want to use
Server: "option" is OK 
### Server - Client : C++ - Java
Client: "required" is needed for parameter you want to use
Server: "option" is OK 
### Server - Client : Java - Java
Client: "option" is OK
Server: "option" is OK

