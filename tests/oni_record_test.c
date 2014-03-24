#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <cv.h>
#include <highgui.h>

#include "utils.h"

void print_property(CvCapture *capture, int property_id, char *str)
{
	printf("%s: %lf", str, cvGetCaptureProperty(capture, property_id));
}

int main(void) 
{
	CvCapture *capture;
	IplImage *depth;
	char key = 0;

	/* set cam */
	capture = cvCaptureFromCAM(CV_CAP_OPENNI);

	if (!capture) {
		die("error initializin openni capture");
	}

	/* need testing : CAP_OPENNI_QVGA_30HZ or CAP_OPENNI_VGA_30HZ ? */
	cvSetCaptureProperty(capture, CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE, CV_CAP_OPENNI_VGA_30HZ);

	/* print properties */
	print_property(capture, CV_CAP_PROP_FRAME_WIDTH, "FRAME_WIDTH");
	print_property(capture, CV_CAP_PROP_FRAME_HEIGHT, "FRAME_HEIGHT");
	print_property(capture, CV_CAP_PROP_OPENNI_FRAME_MAX_DEPTH, "FRAME_MAX_DEPTH");
	print_property(capture, CV_CAP_PROP_FPS, "FPS");


	/* create window */
	cvNamedWindow("MPlayer", CV_WINDOW_AUTOSIZE);

	/* 27 : ESC in ascii... */
	while (key != 'q' && key != 27) {

		cvGrabFrame(capture);

		/* params: capture, stream index*/
		depth = cvRetrieveFrame(capture, CV_CAP_OPENNI_DEPTH_MAP);

		/* display image */
		cvShowImage("MPlayer", depth);

		/* capture key strokes */
		key = cvWaitKey(10);
	}

	cvDestroyAllWindows();
	cvReleaseImage(&depth);
	cvReleaseCapture(&capture);

	return 0;
}