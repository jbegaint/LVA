#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <cv.h>
#include <highgui.h>

#include "matrix.h"
#include "patterns.h"
#include "utils.h"

/* cvNot ?? */
void reverse_data(IplImage *src)
{
	for (int row = 0; row < src->height; row++) {
		/* "2*" dafuq ? */
		for (int col = 0; col < 2 * src->width; col++) {
			 (src->imageData + src->widthStep*row)[col] = 
			 ~(src->imageData + src->widthStep*row)[col] + 1; 
		}
	}
}

void *grab_video(void *arg) 
{
	matrix_t *matrix;
	int *next_frame;
	thread_info_t *thread_info;

	thread_info = (thread_info_t *) arg;
	matrix = thread_info->matrix;
	next_frame = thread_info->next_frame;

	CvCapture *capture;
	IplImage *depth, *small;

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
	small = cvCreateImage(cvSize(N_COLS, N_ROWS), depth->depth, depth->nChannels);

	/* loop */
	while (1) {

		while (*(next_frame) != 1) {
			usleep(100);
		} 

		cvGrabFrame(capture);
		depth = cvRetrieveFrame(capture, CV_CAP_OPENNI_DEPTH_MAP);

		cvResize(depth, small, CV_INTER_LINEAR);

		for (int i = 0; i < small->height; ++i) {
			for (int j = 0; j < small->width; ++j) {
				(matrix->values)[i][j] =
				CV_IMAGE_ELEM(small, unsigned short, i, j);
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