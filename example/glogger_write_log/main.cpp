#include <iostream>
#include <fstream>
#include <string>
#include "glogger/glogger.h"
#include "glogger/gexecutor_filewriter.h"
#include <NIDAQmxBase.h>

#define NUM_THREAD 1000 
#define NUM_ITR 100

using namespace std;

int num=0;

#ifdef WIN32
	HANDLE mutex;
#elif __linux
	pthread_mutex_t mutex;
	int thread_param;
#endif

#define ThreadTest 1 

#ifdef WIN32
	static unsigned WINAPI thread_fuction1(void *_arg) {
#elif __linux__
	static void *thread_function1(void *_arg) {
#endif

#ifdef __linux__
#ifdef ThreadTest
		pthread_mutex_lock(&mutex);
#endif
#endif

		GLogger<string,ofstream> *logger = 
			GLogger<string,ofstream>::getInstance();
		for(int i=0; i<NUM_ITR; i++) {
			cout<<"log1 info "<<num<<endl;
			logger->info(__FILE__,__LINE__,"i1: %d",num);
			num++;	
		}

#ifdef __linux__
#ifdef ThreadTest
		pthread_mutex_unlock(&mutex);
#endif
#endif
		return 0;
	}

#ifdef WIN32
	static unsigned WINAPI thread_fuction2(void *_arg) {
#elif __linux__
	static void *thread_function2(void *_arg) {
#endif

#ifdef __linux__
#ifdef ThreadTest
		pthread_mutex_lock(&mutex);
#endif
#endif

		GLogger<string,ofstream> *logger = 
		GLogger<string,ofstream>::getInstance();
		for(int i=0; i<NUM_ITR; i++) {
			cout<<"log2 info "<<num<<endl;
			logger->info(__FILE__,__LINE__,"i2: %d",num);
			num--;	
		}
#ifdef __linux__
#ifdef ThreadTest
		pthread_mutex_unlock(&mutex);
#endif
#endif

		return 0;
	}

//#define START_STOP_TEST 1
#define LOG_ITERATION 100000

int main(int argc, char *argv[]) {

	int32 error = 0;
	TaskHandle taskHandle = 0;
	char errBuff[2048] = { '\0' };

	DAQmxBaseCreateTask("AccelTask",&taskHandle);

	ofstream *fileout=new ofstream("log.txt",ios::out);
	GLogger<string,ofstream> *logger = 
		GLogger<string,ofstream>::getInstance();
	GExecutorInterface<string,ofstream> *appender = 
		new GExecutorFileWriter<string,ofstream>(fileout,true);
	logger->addAppender("filewriter",appender);
	cout<<"filewriter is appended to logger"<<endl;

	int i=0;
	for(i=0; i<LOG_ITERATION; i++) {
#ifdef START_STOP_TEST
		if(logger->start()>0) {
			cout<<"logger is already started"<<endl;
		}
#endif

		cout<<i<<endl;
		logger->debug(__FILE__,__LINE__,"logging test %d",i);

#ifdef START_STOP_TEST
		if(logger->stop()<0) {
			//cout<<"logger is not stopped"<<endl;
		}
		int j=0;
		while(logger->isRunning()) {
			usleep(1);
			if(j>100) {	
				cout<<"waiting to stop..."<<endl;
			}
			j++;
		}
#endif
	}

#ifdef START_STOP_TEST
	if(logger->start()>0) {
		cout<<"logger is already started"<<endl;
	}
#endif

	cout<<"for() end, iteration is  "<<i<<endl;

#ifdef WIN32
	HANDLE thread_handle[NUM_THREAD];
#elif __linux
	pthread_t thread_handle[NUM_THREAD];
#endif

// create mutex
#ifdef __linux
#ifdef ThreadTest
	pthread_mutex_init(&mutex, NULL);
#endif
#endif

	for(int i=0; i<NUM_THREAD; i++) {
		if(i%2) {
#ifdef WIN32
			thread_handle[i] = 
				(HANDLE)_beginthreadex(
					NULL,
					0,
					thread_function1,
					NULL,0,NULL);
#elif __linux__
			pthread_create(
				&thread_handle[i],
				NULL, 
				thread_function1, 
				NULL); 
#endif
		} else {
#ifdef WIN32
			thread_handle[i] = 
				(HANDLE)_beginthreadex(
					NULL,
					0,
					thread_function2,
					NULL,0,NULL);
#elif __linux__
			pthread_create(
				&thread_handle[i],
				NULL, 
				thread_function2, 
				NULL);
#endif
		}
	}

	for(int i=0; i<NUM_THREAD; i++) {
		pthread_join(thread_handle[i], NULL);
	}

#ifdef ThreadTest
	pthread_mutex_destroy(&mutex);
#endif

	logger->stop();
	while(logger->isRunning()) {
#ifdef WIN32
		Sleep(1);
#elif __linux__
		usleep(100);
#endif	
//		cout<<"This is main, logger is running"<<endl;
	}
	fileout->close();

	cout<< "example is done, num is "<<num<<endl;
	return 0;
}
