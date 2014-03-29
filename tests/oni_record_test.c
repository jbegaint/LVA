#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <cv.h>
#include <highgui.h>

#include "utils.h"

void reverse_data(IplImage *src)
{
	for (int row = 0; row < src->height; row++) {
		/* dafuq ? */
		for (int col = 0; col < 2 * src->width; col++) {
			 (src->imageData + src->widthStep*row)[col] = 
			 ~(src->imageData + src->widthStep*row)[col] + 1; 
		}
	}
}

void print_property(CvCapture *capture, int property_id, char *str)
{
	printf("%s: %lf\n", str, cvGetCaptureProperty(capture, property_id));
}

int main(void) 
{
	CvCapture *capture;
	IplImage *depth, *show, *small;

	char key = 0;


	/* set cam */
	capture = cvCaptureFromCAM(CV_CAP_OPENNI);

	if (!capture) {
		die("error initializing openni capture\n");
	}

	/* need testing : CAP_OPENNI_QVGA_30HZ or CAP_OPENNI_VGA_30HZ  */
	cvSetCaptureProperty(capture, CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE, CV_CAP_OPENNI_VGA_30HZ);

	/* print properties */
	print_property(capture, CV_CAP_PROP_FRAME_WIDTH, "FRAME_WIDTH");
	print_property(capture, CV_CAP_PROP_FRAME_HEIGHT, "FRAME_HEIGHT");
	print_property(capture, CV_CAP_PROP_OPENNI_FRAME_MAX_DEPTH, "FRAME_MAX_DEPTH");
	print_property(capture, CV_CAP_PROP_FPS, "FPS");

	/* create window */
	cvNamedWindow("MPlayer", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("MPlayer1", CV_WINDOW_AUTOSIZE);


	/* first run */
	cvGrabFrame(capture);
	depth = cvRetrieveFrame(capture, CV_CAP_OPENNI_DEPTH_MAP);
	show = cvCreateImage(cvSize(depth->width, depth->height), depth->depth, depth->nChannels);
	small = cvCreateImage(cvSize(25, 14), depth->depth, depth->nChannels);

	/* 27 : ESC in ascii... */
	while (key != 'q' && key != 27) {

		cvGrabFrame(capture);

		/* params: capture, stream index*/
		depth = cvRetrieveFrame(capture, CV_CAP_OPENNI_DEPTH_MAP);

		cvConvertScale(depth, show, 10, 0);
		reverse_data(show);

		cvResize(show, small, CV_INTER_LINEAR);

		/*cvEqualizeHist(fat, fat);*/

		/* display images */
		cvShowImage("MPlayer", show);

		/* capture key strokes */
		key = cvWaitKey(10);
	}

	cvDestroyAllWindows();

	/* todo: fix free segfault */
	/*cvReleaseImage(&depth);*/

	cvReleaseCapture(&capture);

	return 0;
}