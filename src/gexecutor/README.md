### Directory Structure
```
├── src
    └── gexecutor
        └── gexecutor_filewriter.h
        └── gexecutor_thrift.h
```

### GExecutorFileWriter
This executor is to write string to file line by line or continuously.
typename <T, E> is <string, ofstring> or <wstring, wofstring>

### GExecutorThrift
This executor is to send message to Thrift server or client.
