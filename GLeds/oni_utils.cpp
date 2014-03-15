#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
 
// OpenNI Header
#include <XnCppWrapper.h>

#include "oni_utils.h"

/* link to original author ?? */

#ifdef __cplusplus
	extern "C" {
#endif 
void convert_frames(matrix_t *matrix, char *filepath)
{
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
	for( unsigned int i = 0; i < uFrames; ++ i )
	{
		xDepthGenerator.WaitAndUpdateData();
		cout << i << "/" << uFrames << endl;
 
		// get value
		xn::DepthMetaData xDepthMap;
		xDepthGenerator.GetMetaData( xDepthMap );

		for( unsigned int y = 0; y < xDepthMap.YRes(); ++ y )
		{
			for( unsigned int x = 0; x < xDepthMap.XRes(); ++ x )
			{
				cout << xDepthMap( x, y ) << endl;
			}
		}
	}
 
	// stop
	xContext.StopGeneratingAll();
 
	// release resource
	xContext.Release();
}
#ifdef __cplusplus
	}
#endif 