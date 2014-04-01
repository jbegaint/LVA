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

int next_frame;

#ifdef __cplusplus
	extern "C" {
#endif 
void *convert_frames(void *arg)
{
	const char *filepath;
	matrix_t *matrix;
	thread_info_t *thread_info;
	int *next_frame;

	thread_info = (thread_info_t *) arg;
	matrix = thread_info->matrix;
	filepath = thread_info->filepath;
	next_frame = thread_info->next_frame;

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
 
	// Start
	xContext.StartGeneratingAll();
 
	// main loop
	for (unsigned int i = 0; i < uFrames; ++i) {

		/* waiting for the worms to come... */
		while (*(next_frame) != 1) {
			usleep(1000);
		}

		xDepthGenerator.WaitAndUpdateData();
 
		// get value
		xn::DepthMetaData xDepthMap;
		xDepthGenerator.GetMetaData(xDepthMap);

		for (unsigned int y = 0; y < xDepthMap.YRes(); ++y) {
			for (unsigned int x = 0; x < xDepthMap.XRes(); ++x) {
				/* reverse mat */
				(matrix->values)[y][x] = xDepthMap(x, y);
			}
		}

		/* pause conversion */
		*next_frame = 0;

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
