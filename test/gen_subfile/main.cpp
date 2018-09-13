#include <iostream>
#include <stdarg.h>
#include <time.h>
#include "gtaskque/gtaskque.h"
#include "glogger/glogger.h"
#include "glogger/gtask_filewriter.h"
#include "gen_subfile/gen_subfile.h"

using namespace std;


// parameters
// path
// filename
// delimiter
// output path
// postfix : postfix of a output file

void wmain(int argc, wchar_t *argv[]) {
#ifdef WIN32
	_CrtDumpMemoryLeaks();
#endif

	GLogger<string> *logger = GLogger<string>::getInstance();
	ofstream logout("./log.txt", ios::out);
	GTaskInterface<string> *appender = new GTaskFileWriter<string>(&logout, false);
	logger->addAppender("appender", appender);

	logger->start();

	const int nArgs = 7;
	
	wstring path;
	wstring filename;
	wstring delimiter = L",";
	wstring output_path;
	wstring postfix;

	if (argc > nArgs) {
		path = argv[1];         // 1
		filename = argv[2];     // 2
		delimiter = argv[3];    // 3
		output_path = argv[4];  // 4
		postfix = argv[5];      // 5

		// 7 variables
		wcout << L"argc: " << argc << endl;
		wcout << L"argv[0]: curr_file_path: " << argv[0] << endl;
		wcout << L"argv[1]: path: " << argv[1] << endl;
		wcout << L"argv[2]: filename: " << argv[2] << endl;
		wcout << L"argv[3]: delimiter: " << argv[3] << endl;
		wcout << L"argv[4]: output_path: " << argv[4] << endl;
		wcout << L"argv[5]: postfix: " << argv[5] << endl;
	}
	else {
		cout << "at least 7 arguments are needed" << endl;
		cout << "path filename delimiter outputpath postfix nCol ...column_indexes" << endl;
		return;
	}
	wcout << "argv[6]: num_of_arg: " << argv[6] << endl;
	list<int> column_indexes;
	int max_column_index = 0;
	for (int i = 0; i < argc - nArgs; i++) {
		int arg = stoi(argv[i + nArgs]);
		wcout << "arg: criterion_index " << arg << endl;
		
		column_indexes.push_back(arg);
		max_column_index = __max(max_column_index, arg);
	}

	if (size_t(stoi(argv[nArgs - 1])) != column_indexes.size()) {
		cout << "The size of col_indexes is defined incorrectly" << endl;
		return;
	}

	clock_t tick = clock();

	bool skip_first_line = true;
	
	try {
		SubFileExtractor::extract_subfiles(
			path,
			filename,
			delimiter,
			skip_first_line,
			column_indexes,
			max_column_index,
			output_path,
			postfix);
	}
	catch (string e) {
		cout << "try error : " << e << endl;
	}

	
	cout << "elapsed time (sec): " << (float)(clock() - tick) / CLOCKS_PER_SEC << endl;
	tick = clock();

	cout << "any input + enter to quit()" << endl;
	int n;
	cin >> n;

	logger->stop();
}
