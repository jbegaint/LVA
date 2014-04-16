#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#include <XnOpenNI.h>

#include "../BBBIOlib/BBBio_lib/BBBiolib.h"

#include "matrix.h"
#include "xn_record.h"
#include "patterns.h"
#include "pins.h"
#include "utils.h"

int N_LAPS;
matrix_t **matrixes;

void println(char *str)
{
	printf("%s\n", str);
}

double time_spent(clock_t start, clock_t end)
{
	return ((double)(end - start) / CLOCKS_PER_SEC);
}

/* video bench, takes numbers of laps as argument */
double bench_video(void)
{
	/* openni related stuff */
	XnStatus nRetVal = XN_STATUS_OK;
	XnContext* pContext;
	XnNodeHandle hScriptNode;
	XnEnumerationErrors* pErrors;
	XnNodeHandle hDepth;
	XnDepthMetaData* pDepthMD;
	const XnDepthPixel* pDepthMap;
	const char *fn = NULL;

	/* stuff we need for the benchmark */
	matrix_t *matrix;
	clock_t start, end;
	double tmp, avg_time_spent = 0;

	nRetVal = xnEnumerationErrorsAllocate(&pErrors);
	CHECK_RC(nRetVal, "Allocate errors object");

	if (fileExists(SAMPLE_XML_PATH_LOCAL)) {
		fn = SAMPLE_XML_PATH_LOCAL;
	}
	else {
		die("Could not find '%s'. Aborting.\n", SAMPLE_XML_PATH_LOCAL);
	}

	printf("Reading config from: '%s'\n", fn);
	nRetVal = xnInitFromXmlFileEx(fn, &pContext, pErrors, &hScriptNode);

	if (nRetVal == XN_STATUS_NO_NODE_PRESENT) {
		XnChar strError[1024];
		xnEnumerationErrorsToString(pErrors, strError, 1024);
		die("%s\n", strError);
		xnEnumerationErrorsFree(pErrors);
	}
	else if (nRetVal != XN_STATUS_OK) {
		die("Open failed: %s\n", xnGetStatusString(nRetVal));
		xnEnumerationErrorsFree(pErrors);
	}

	xnEnumerationErrorsFree(pErrors);

	nRetVal = xnFindExistingRefNodeByType(pContext, XN_NODE_TYPE_DEPTH, &hDepth);
	CHECK_RC(nRetVal, "Find depth generator");

	pDepthMD = xnAllocateDepthMetaData();
	
	for (int c = 0; c < N_LAPS; ++c) {

		matrix = matrixes[c];

		start = clock();

		/* grab image */
		nRetVal = xnWaitOneUpdateAll(pContext, hDepth);

		if (nRetVal != XN_STATUS_OK) {
			printf("UpdateData failed: %s\n", xnGetStatusString(nRetVal));
			continue;
		}

		xnGetDepthMetaData(hDepth, pDepthMD);

		end = clock();

		pDepthMap = pDepthMD->pData;

		/* copy tmp_matrix */
		for (unsigned int i = 0; i < pDepthMD->pMap->Res.Y; ++i) {
			for (unsigned int j = 0; j < pDepthMD->pMap->Res.X; ++j) {
				(matrix->values)[i][j] = pDepthMap[i * pDepthMD->pMap->Res.X + j];
			}
		}

		tmp = time_spent(start, end);
		printf("lap %d, time spent: %lf\n", c, tmp);
		avg_time_spent += tmp;

		/* wait 10ms */
		usleep(10 * 1000);

	}

	printf("average time: %lf\n", avg_time_spent / N_LAPS);

	xnFreeDepthMetaData(pDepthMD);

	xnProductionNodeRelease(hDepth);
	xnProductionNodeRelease(hScriptNode);
	xnContextRelease(pContext);

	return avg_time_spent;
}

double bench_mat_operations(void)
{
	clock_t start, end;
	matrix_t *tmp_mat, *res;
	double tmp, avg_time_spent = 0;

	/* we stock the results of the operation in res */
	res = init_matrix(N_ROWS, N_COLS);

	for (int c = 0; c < N_LAPS; ++c) {

		start = clock();

		tmp_mat = get_resized_matrix(matrixes[c], N_ROWS, N_COLS);

		center_and_threshold_matrix(tmp_mat);
		copy_matrix(res, tmp_mat);
		free_matrix(tmp_mat);

		end = clock();
		tmp = time_spent(start, end);
		printf("lap %d, time spent: %lfus\n", c, tmp * 1000 * 1000);
		avg_time_spent += tmp;

		reset_matrix(res);
		/* wait 10ms */
		usleep(10 * 1000);
	}

	printf("\naverage time: %lf us (matrixes operations)\n", avg_time_spent / N_LAPS * 1000 * 1000);
	return avg_time_spent;
}

int main(int argc, char **argv)
{
	double avg_video, avg_mat;

	/* arg parsing */
	if (argc > 2) {
		die("error: missing argument or bad usage: %s [n_laps] \n", argv[0]);
	}
	else if (argc == 2) {
		N_LAPS = atoi(argv[1]);
	}
	else {
		/* default value */
		N_LAPS = 10;
	}
	
	/* todo: fix mem leak */
	matrixes = (matrix_t **) calloc(N_LAPS, sizeof(matrix_t *));

	/* init matrixes */
	for (int c = 0; c < N_LAPS; ++c) {
		matrixes[c] = init_matrix(PIXELS_Y, PIXELS_X);
	}

	println("=== Depth data capture ===");
	avg_video = bench_video();

	println("\n=== Matrix operations ===");
	avg_mat = bench_mat_operations();

	/* free matrixes */
	for (int c = 0; c < N_LAPS; ++c) {
		free_matrix(matrixes[c]);
	}
	free(matrixes);

	println("\n=== Resume ===");
	printf("average time: %lf us (video operations)\n", avg_video / N_LAPS * 1000 * 1000);
	printf("average time: %lf us (matrixes operations)\n", avg_mat / N_LAPS * 1000 * 1000);

	return 0;
}