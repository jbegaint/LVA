#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <cv.h>
#include <highgui.h>

#include "matrix.h"
#include "patterns.h"
#include "utils.h"

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
	IplImage *depth, *show, *small;
	int flag = 1;

	time_t timer;
	char buffer[25];
	struct tm* tm_info;

	CvMat *my_mat = cvCreateMat(N_COLS, N_ROWS, CV_8UC1);

	/* set cam */
	capture = cvCaptureFromCAM(CV_CAP_OPENNI);

	if (!capture) {
		die("error initializing openni capture\n");
	}
	printf("openni capture initialized\n");

	/* need testing : CAP_OPENNI_QVGA_30HZ or CAP_OPENNI_VGA_30HZ  */
	cvSetCaptureProperty(capture, CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE, CV_CAP_OPENNI_VGA_30HZ);

	while (1) {

		while (*(next_frame) != 1) {
			usleep(1000);
		} 

		time(&timer);
		tm_info = localtime(&timer);
		strftime(buffer, 25, "%H:%M:%S", tm_info);
		printf("\rnext_frame: %s", buffer);
		fflush(stdout);

		cvGrabFrame(capture);

		/* params: capture, stream index*/
		depth = cvRetrieveFrame(capture, CV_CAP_OPENNI_DEPTH_MAP);


		if (flag) {
			show = cvCreateImage(cvSize(depth->width, depth->height), depth->depth, depth->nChannels);

			small = cvCreateImage(cvSize(25, 14), depth->depth, depth->nChannels);
			flag = 0;
		}

		cvResize(depth, small, CV_INTER_LINEAR);
		cvCopy(my_mat, depth, NULL);

		for (int i = 0; i < my_mat->rows; ++i) {
			for (int j = 0; j < my_mat->cols; ++j) {
				(matrix->values)[i][j] = ((my_mat->data).ptr)[i*my_mat->rows + j];
			}
		}

		*next_frame = 0;

	}

	/* todo: fix free segfault */
	/*cvReleaseImage(&depth);*/

	/* FREE THE MALLOCS ? */

	cvReleaseCapture(&capture);

	return NULL;
}