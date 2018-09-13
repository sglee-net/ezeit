/*********************************************************************
*
* ANSI C Example program:
*    ContAccelSamps-IntClk-AnlgStart.c
*
* Example Category:
*    AI
*
* Description:
*    This example demonstrates how to create an analog input
*    acceleration task and perform a continuous acquisition using
*    option IEPE excitation, analog triggering, and overload
*    detection.
*
* Instructions for Running:
*    1. Select the physical channel to correspond to where your
*       signal is input on the device.
*    2. Enter the minimum and maximum expected acceleration values.
*    Note: To optimize gain selection, try to match the Input Ranges
*          to the expected level of the measured signal.
*    3. Program the analog input terminal configuration and IEPE
*       excitation settings for your device.
*    4. If your device supports overload detection, check the
*       Overload Detection checkbox. Refer to your device
*       documentation to see if overload protection is supported.
*    5. Set the rate of the acquisition. Also set the Samples to Read
*       control. This will determine how many samples are read at a
*       time. This also determines how many points are plotted on the
*       graph each time.
*    Note: The rate should be at least twice as fast as the maximum
*          frequency component of the signal being acquired.
*    6. Set the source of the Analog Edge Start Trigger. By default
*       this is Dev1/ai0.
*    7. Set the slope and level of desired analog edge condition.
*    8. Set the Hysteresis Level.
*    9. Input the sensitivity and units for your accelerometer.
*
* Steps:
*    1. Create a task.
*    2. Create an analog input acceleration channel. This step
*       defines accelerometer sensitivity, desired range, and IEPE
*       excitation.
*    3. Set the sample rate and define a continuous acquisition.
*    4. Define the trigger channel, trigger level, rising/falling
*       edge, and hysteresis window for an analog start trigger.
*    5. Call the Start function to start the acquisition.
*    6. Read the waveform data in the EveryNCallback function until
*       the user hits the stop button or an error occurs.
*    7. Check for overloaded channels.
*    8. Call the Clear Task function to clear the Task.
*    9. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input terminal matches the Physical
*    Channel I/O control. Also, make sure your analog trigger
*    terminal matches the Trigger Source Control. For further
*    connection information, refer to your hardware reference manual.
*
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <NIDAQmx.h>
#include <time.h>
#include <chrono>
#include <unistd.h>
#include <math.h>
#include "./glogger/glogger.h"
#include "./glogger/gexecutor_filewriter.h"
#include <string>
#include <thread>
#include <iostream>
//#include <crtdbg.h>
//#include <direct.h>

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

#define nCHANNEL 4
#define nSAMPLES 10000
#define nSIZE nCHANNEL*nSAMPLES

using namespace std;
using namespace std::chrono;

#define maxFilePathLength 256

void getCurrentDirectory(string &_path) {
	char *strBuffer = new char[maxFilePathLength];
	char *rt = nullptr;
#ifdef WIN32
	rt = _getcwd(strBuffer, maxFilePathLength);
#elif __linux__
	rt = getcwd(strBuffer, maxFilePathLength);
#endif

	if (rt) {
		_path = rt;
	}

	delete strBuffer;
}

int main(void)
{
	std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
	std::chrono::duration<double> sec;

	string current_path;
	::getCurrentDirectory(current_path);
	current_path = current_path + "\\accel.csv";

	ofstream *fileout = new ofstream(current_path, ios::out);
	GLogger<string, ofstream> *logger =
		GLogger<string, ofstream>::getInstance();
	GExecutorInterface<string, ofstream> *appender =
		new GExecutorFileWriter<string, ofstream>(fileout, true);
	logger->addAppender("filewriter", appender);
	cout << "filewriter is appended to logger" << endl;

	int32       error = 0;
	TaskHandle  taskHandle = 0;
	char        errBuff[2048] = { '\0' };

	/*********************************************/
	// DAQmx Configure Code
	/*********************************************/
	DAQmxErrChk( DAQmxCreateTask("AccelTask", &taskHandle) );
	printf("1\n");

	DAQmxErrChk( 
		DAQmxCreateAIAccelChan(
			taskHandle, 
			"cDAQ1Mod1/ai0:3", "", 
			DAQmx_Val_PseudoDiff, 
			-100.0, 100.0, 
			DAQmx_Val_AccelUnit_g, 
			50, 
			DAQmx_Val_mVoltsPerG, 
			DAQmx_Val_Internal, 
			0.004, 
			NULL)
		);
	printf("2\n");

	// rate 10K
	DAQmxErrChk( 
		DAQmxCfgSampClkTiming(
			taskHandle, "", 
			10000.0, 
			DAQmx_Val_Rising, 
			DAQmx_Val_ContSamps, 
			nSAMPLES)
		);
	printf("3\n");

////		DAQmxCfgAnlgEdgeStartTrig(taskHandle,"cDAQ1Mod1/ai1",DAQmx_Val_Rising,0.1);
//	printf("4\n");
////		DAQmxSetAnlgEdgeStartTrigHyst(taskHandle, 4.0);
//	printf("5\n");

	DAQmxErrChk( 
		DAQmxRegisterEveryNSamplesEvent(
			taskHandle, 
			DAQmx_Val_Acquired_Into_Buffer,
			nSAMPLES, 
			0, 
			EveryNCallback, 
			NULL)
		);
	printf("6\n");
	DAQmxErrChk( 
		DAQmxRegisterDoneEvent(
			taskHandle, 
			0, 
			DoneCallback, 
			NULL)
		);
	printf("7\n");

	/*********************************************/
	// DAQmx Start Code
	/*********************************************/
	DAQmxStartTask(taskHandle);
	printf("8\n");

	printf("Acquiring samples continuously. Press Enter to interrupt\n");
	getchar();

Error:
	if (DAQmxFailed(error))
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	if (taskHandle != 0) {
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
	if (DAQmxFailed(error))
		printf("DAQmx Error: %s\n", errBuff);
	printf("End of program, press Enter key to quit\n");
	getchar();
	return 0;
}
static int ggg = 0;

int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
	int32       error = 0;
	char        errBuff[2048] = { '\0' };
	static int  totalRead = 0;
	int32       read = 0;
	float64     data[nSIZE];
	/* Change this variable to 1 if you are using a DSA device and want to check for Overloads. */
	int32       overloadDetectionEnabled = 0;
	bool32      overloaded = 0;
	char        overloadedChannels[nSIZE];

	/*********************************************/
	// DAQmx Read Code
	/*********************************************/
	//	printf("Read\n");
	float64 timeout = 10.0;
	DAQmxReadAnalogF64(taskHandle, nSAMPLES, timeout, DAQmx_Val_GroupByScanNumber, data, nSIZE, &read, NULL);
	//	printf("Read Done\n");
	if (overloadDetectionEnabled) {
		DAQmxGetReadOverloadedChansExist(taskHandle, &overloaded);
	}

	GLogger<string, ofstream> *logger = 
		GLogger<string, ofstream>::getInstance();
	time_t t = time(nullptr);

	if (read > 0) {
		totalRead += read;
//		printf("%I64d \n", ms.count());
//		int count = nSAMPLES / nCHANNEL;
		printf("%d \n", (long)t);
		for (int i = 0; i <nSIZE; i=i+nCHANNEL) {
			logger->write("%d, %.5f, %.5f, %.5f, %.5f", (long)t, (float)data[i+0], (float)data[i+1], (float)data[i+2], (float)data[i+3]);
		}
		printf("Acquired %d samples. Total %d, Data[0] %2.5f, Data[1] %2.5f, Data[2] %2.5f, Data[3] %2.5f \r", (int)read, (int)totalRead, fabs(data[0]), fabs(data[1]), fabs(data[2]), fabs(data[3]));
	}
//	printf("%d\n", i);
//	logger->write("");

	if (overloaded) {
		DAQmxGetReadOverloadedChans(taskHandle, overloadedChannels, nSIZE);
		printf("Overloaded channels: %s\n", overloadedChannels);
	}
	fflush(stdout);

Error:
	if (DAQmxFailed(error)) {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
		printf("DAQmx Error: %s\n", errBuff);
	}
	return 0;
}

int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData)
{
	int32   error = 0;
	char    errBuff[2048] = { '\0' };

	// Check to see if an error stopped the task.
	//DAQmxErrChk(status);

Error:
	if (DAQmxFailed(error)) {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		DAQmxClearTask(taskHandle);
		printf("DAQmx Error: %s\n", errBuff);
	}
	return 0;
}
