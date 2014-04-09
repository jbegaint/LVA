#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#ifdef OPENCV
#include <cv.h>
#include <highgui.h>
#endif

#include "matrix.h"
#include "patterns.h"
#include "utils.h"


#ifdef OPENCV
void *grab_video(void *arg) 
{
	matrix_t *matrix;
	int *next_frame;
	thread_info_t *thread_info;

	thread_info = (thread_info_t *) arg;
	matrix = thread_info->matrix;
	next_frame = thread_info->next_frame;

	CvCapture *capture;
	IplImage *depth;

	/* set cam */
	capture = cvCaptureFromCAM(CV_CAP_OPENNI);

	if (!capture) {
		die("error initializing openni capture\n");
	}
	printf("openni capture initialized\n");

	/* need testing : CAP_OPENNI_QVGA_30HZ or CAP_OPENNI_VGA_30HZ  */
	cvSetCaptureProperty(capture, CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE, CV_CAP_OPENNI_VGA_30HZ);

	/* first run*/
	cvGrabFrame(capture);
	/* params: capture, stream index*/
	depth = cvRetrieveFrame(capture, CV_CAP_OPENNI_DEPTH_MAP);

	/* loop */
	while (1) {

		while (*(next_frame) != 1) {
			usleep(100);
		} 

		cvGrabFrame(capture);
		depth = cvRetrieveFrame(capture, CV_CAP_OPENNI_DEPTH_MAP);

		for (int i = 0; i < depth->height; ++i) {
			for (int j = 0; j < depth->width; ++j) {
				(matrix->values)[i][j] = CV_IMAGE_ELEM(depth, unsigned short, i, j);
			}
		}

		*next_frame = 0;

	}

	/* CAUTION: one does not simply exit a while 1 */

	/* todo: fix free segfault */
	/*cvReleaseImage(&depth);*/

	/* FREE THE MALLOCS ? */

	/*cvReleaseCapture(&capture);
	free_matrix(matrix);
*/
	return NULL;
}
#endif
