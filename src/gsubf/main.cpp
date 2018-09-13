#include <iostream>
#include <stdarg.h>
#include <time.h>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include "gtaskque/gtaskque.h"
#include "glogger/glogger.h"
#include "glogger/gexecutor_filewriter.h"
#include "gsubf.h"

using namespace std;
using namespace rapidjson;

rapidjson::Document parseJsonObject(
         const string &_path,
         const string &_file_name) {
         string file_path = _path + _file_name;
 
         FILE *fp = fopen(file_path.c_str(),"r");
         if(!fp) {
                 throw("file open failed");
         }
 
         char readBuffer[65536];
         FileReadStream is(fp, readBuffer, sizeof(readBuffer));
 
         Document doc;
         try {
                 doc.ParseStream(is);
                 fclose(fp);
	} catch(string &e) {
		cout<<"error is occurred"<<endl;
                throw(e);
        }

	        return doc;
}

void getAttributeFromJsonObject(
        string &_path,
        string &_file_name,
        string &_delimiter,
        string &_output_path,
        string &_postfix,
        list<int> &_reference_index,
	bool &_skip_first_line,
        const rapidjson::Document &_doc) {
        assert(_doc.IsObject());
        if(!_doc.IsObject()) {
		throw("json doc is not Object");
        }

        const Value &path = _doc["path"];
        const Value &file_name = _doc["file_name"];
        const Value &delimiter = _doc["delimiter"];
	const Value &output_path = _doc["output_path"];
        const Value &postfix = _doc["postfix"];
	const Value &reference_index = _doc["reference_index"];
	const Value &skip_first_line = _doc["skip_first_line"];

        assert(path.IsString());
        if(!path.IsString()) {
                throw("path is not String type");
        }
        _path = path.GetString();

        assert(file_name.IsString());
        if(!file_name.IsString()) {
		throw("file_name is not String type");
        }
        _file_name = file_name.GetString();

        assert(delimiter.IsString());
        if(!delimiter.IsString()) {
                throw("delimiter is not String type");
        }
        _delimiter = delimiter.GetString();

        assert(output_path.IsString());
	if(!output_path.IsString()) {
                throw("outputpath is not String type");
        }
        _output_path = output_path.GetString();

        assert(postfix.IsString());
        if(!postfix.IsString()) {
		throw("postfix is not String type");
        }
        _postfix = postfix.GetString();

        assert(reference_index.IsArray());
        if(!reference_index.IsArray()) {
                throw("reference index is not Array type");
        }
        for(SizeType i=0; i< reference_index.Size(); i++) {
                assert(reference_index[i].IsInt());
                if(!reference_index[i].IsInt()) {
                        throw("reference index is not Int type");
                }
                _reference_index.push_back(reference_index[i].GetInt());
	}

	assert(skip_first_line.IsBool());
	if(!skip_first_line.IsBool()) {
		throw("skip_first_line is not Bool type");
	}
	_skip_first_line = skip_first_line.GetBool();
}



// filename
// delimiter
// output path
// postfix : postfix of a output file

int main(int argc, char *argv[]) {
#ifdef WIN32
	_CrtDumpMemoryLeaks();
#endif

	GLogger<string,ofstream> *logger = 
		GLogger<string,ofstream>::getInstance();
	ofstream *logout = 
		new ofstream("./log.txt", ios::out);
	GExecutorInterface<string,ofstream> *appender = 
		new GExecutorFileWriter<string,ofstream>(logout,true);
	logger->addAppender("appender", appender);

	// log
	cout<<"argc "<<argc<<endl;
	cout<<"argv[0] "<<argv[0]<<endl;
	logger->debug(__FILE__,__LINE__,"argc: %d",argc);
	logger->debug(__FILE__,__LINE__,"argv: %s",argv[0]);

        string path="";
        string file_name = string(argv[0]) + ".json";

        string delimiter,output_path,postfix;
        list<int> reference_index;
	bool skip_first_line = true;

        Document doc;
        try {
 		doc = parseJsonObject(path,file_name);
                getAttributeFromJsonObject(
                        path,
                        file_name,
                        delimiter,
                        output_path,
                        postfix,
                        reference_index,
			skip_first_line,
                        doc);
        } catch (string &e) {
		cerr<<e;
        }

	// log
	logger->debug(__FILE__,__LINE__,"path: %s",path.c_str());
	logger->debug(__FILE__,__LINE__,"file_name: %s",file_name.c_str());
	logger->debug(__FILE__,__LINE__,"delimiter: %s",delimiter.c_str());
	logger->debug(__FILE__,__LINE__,"postfix: %s",postfix.c_str());
	for_each(
		reference_index.begin(),
		reference_index.end(),
		[&](int &i) {
			logger->debug(__FILE__,__LINE__,"index: %d",i);
		});

	if(output_path.at(output_path.size()-1)!='/') {
		output_path = output_path + '/';
		logger->info(__FILE__,__LINE__,"/ is add to output_path");
	}

        // 3. Stringify the DOM
        //StringBuffer buffer;
        //Writer<StringBuffer> writer(buffer);
        //doc.Accept(writer);
        //std::cout << buffer.GetString() << std::endl;

	// log
	logger->info(__FILE__,__LINE__,"parsing is done");

	clock_t tick = clock();

	try {
		GSubf::gen_subfiles(
			path,
			file_name,
			delimiter,
			skip_first_line,
			reference_index,
			output_path,
			postfix);
	}
	catch (string e) {
		logger->error(__FILE__,__LINE__,"throws in get_subfiles: %s", e.c_str());
	}

	// log
	logger->info(__FILE__,__LINE__,"subfile generation is done");

	float elapsed_time = (float)(clock() - tick) / CLOCKS_PER_SEC;	
	tick = clock();
	
	cout << "elapsed time (sec): " << elapsed_time << endl;
	logger->info(__FILE__,__LINE__,"elpsed time(sec): %f",elapsed_time);

	// stop the logger
	logger->stop();
	while(logger->isRunning()) {
		usleep(1000);
	}
	
	return 0;
}
