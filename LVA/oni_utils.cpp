#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
 
// OpenNI Header
#include <XnCppWrapper.h>

#include <unistd.h>
#include "matrix.h"
#include "oni_utils.h"
#include "patterns.h"

extern int next_frame;

#ifdef __cplusplus
	extern "C" {
#endif 
void *convert_frames(void *arg)
{
	thread_info_t *thread_info;
	matrix_t *matrix;
	const char *filepath;

	thread_info = (thread_info_t *) arg;
	/* segfault here */
	matrix = thread_info->matrix;
	filepath = thread_info->filepath;

	/* opening file */
	cout << "File loaded: " << filepath << endl;

	// Initial OpenNI Context
	xn::Context xContext;
	xContext.Init();
 
	// open ONI file
	xn::Player xPlayer;
	xContext.OpenFileRecording( filepath, xPlayer );
	xPlayer.SetRepeat( false );
 
	// Create depth generator
	xn::DepthGenerator xDepthGenerator;
	xDepthGenerator.Create( xContext );
 
	// get total frame number
	XnUInt32 uFrames;
	xPlayer.GetNumFrames( xDepthGenerator.GetName(), uFrames );
	cout << "Total " << uFrames << " frames" << endl;
 
	// Start
	xContext.StartGeneratingAll();
 
	// main loop
	for (unsigned int i = 0; i < uFrames; ++i) {

		/* waiting for the worms to come... */
		while (1) {
			/* sleep 1 ms */
			usleep(1000); 
			if (next_frame == 1) {
				break;
			}
		}

		xDepthGenerator.WaitAndUpdateData();
		cout << "Frame: " << i << "/" << uFrames << endl;
 
		// get value
		xn::DepthMetaData xDepthMap;
		xDepthGenerator.GetMetaData(xDepthMap);

		for (unsigned int y = 0; y < xDepthMap.YRes(); ++y) {
			for (unsigned int x = 0; x < xDepthMap.XRes(); ++x) {
				(matrix->values)[y][x] = xDepthMap(x, y);
			}
		}

		/* pause conversion */
		next_frame = 0;

	}
 
	// stop
	xContext.StopGeneratingAll();
 
	// release resource
	xContext.Release();

	return NULL;
}
#ifdef __cplusplus
	}
#endif 