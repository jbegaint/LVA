/*
	Mostly based on : https://github.com/OpenNI/OpenNI/blob/master/Samples/NiCRead/NiCRead.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <XnOpenNI.h>

#include "xn_record.h"
#include "matrix.h"
#include "patterns.h"
#include "utils.h"

XnBool fileExists(const char *fn)
{
	XnBool exists;
	xnOSDoesFileExist(fn, &exists);
	return exists;
}

void *xngrab_video(void *arg)
{
	XnStatus nRetVal = XN_STATUS_OK;
	XnContext* pContext;
	XnNodeHandle hScriptNode;
	XnEnumerationErrors* pErrors;
	XnNodeHandle hDepth;
	XnDepthMetaData* pDepthMD;
	const XnDepthPixel* pDepthMap;
	XnDepthPixel middlePoint;
	const char *fn = NULL;

	/* get parameters */
	matrix_t *matrix;
	int *thread_status;
	thread_info_t *thread_info;

	thread_info = (thread_info_t *) arg;
	matrix = thread_info->matrix;
	thread_status = thread_info->thread_status;

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
	
	while (*(thread_status) != THREAD_QUIT) {

		while (*(thread_status) == THREAD_PAUSED) {
			usleep(100);
		}

		/* grab image */
		nRetVal = xnWaitOneUpdateAll(pContext, hDepth);

		if (nRetVal != XN_STATUS_OK) {
			printf("UpdateData failed: %s\n", xnGetStatusString(nRetVal));
			continue;
		}

		xnGetDepthMetaData(hDepth, pDepthMD);
		pDepthMap = pDepthMD->pData;

		/* copy matrix */
		for (unsigned int i = 0; i < pDepthMD->pMap->Res.Y; ++i) {
			for (unsigned int j = 0; j < pDepthMD->pMap->Res.X; ++j) {
				(matrix->values)[i][j] = pDepthMap[i * pDepthMD->pMap->Res.X + j];
			}
		}

		/* wait */
		*thread_status = THREAD_PAUSED;

	}

	fprintf(stderr, "closing video capture\n");

	xnFreeDepthMetaData(pDepthMD);

	xnProductionNodeRelease(hDepth);
	xnProductionNodeRelease(hScriptNode);
	xnContextRelease(pContext);

	return NULL;
}
