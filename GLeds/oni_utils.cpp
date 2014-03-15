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
void convert_frames(char *filepath)
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
 /*
		stringstream sName;
		sName << "Frame_" << i << ".txt";
		ofstream oFile( sName.str().c_str() );
		for( unsigned int y = 0; y < xDepthMap.YRes(); ++ y )
		{
			for( unsigned int x = 0; x < xDepthMap.XRes(); ++ x )
			{
				oFile << xDepthMap( x, y ) << " ";
			}
			oFile << "\n";
		}
		oFile.close();*/
	}
 
	// stop
	xContext.StopGeneratingAll();
 
	// release resource
	xContext.Release();
}
#ifdef __cplusplus
	}
#endif 