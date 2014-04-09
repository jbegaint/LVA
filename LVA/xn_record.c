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
	XnContext *pContext;
	XnNodeHandle hScriptNode;
	XnEnumerationErrors *pErrors;
	XnNodeHandle hDepth;
	XnDepthMetaData *pDepthMD;
	const XnDepthPixel *pDepthMap;
	XnDepthPixel middlePoint;
	const char *fn = NULL;

	nRetVal = xnEnumerationErrorsAllocate(&pErrors);
	CHECK_RC(nRetVal, "Allocate errors object");

	if (fileExists(SAMPLE_XML_PATH)) {
		fn = SAMPLE_XML_PATH;
	}
	else if (fileExists(SAMPLE_XML_PATH_LOCAL)) {
		fn = SAMPLE_XML_PATH_LOCAL;
	} 
	else {
		printf("Could not find '%s' nor '%s'. Aborting.\n", SAMPLE_XML_PATH,
			SAMPLE_XML_PATH_LOCAL);
		/* todo: handle error */
		return XN_STATUS_ERROR;
	}

	printf("Reading config from: '%s'\n", fn);
	nRetVal = xnInitFromXmlFileEx(fn, &pContext, pErrors, &hScriptNode);

	if (nRetVal == XN_STATUS_NO_NODE_PRESENT) {
		XnChar strError[1024];
		xnEnumerationErrorsToString(pErrors, strError, 1024);
		printf("%s\n", strError);
		xnEnumerationErrorsFree(pErrors);
		return (nRetVal);
	} 
	else if (nRetVal != XN_STATUS_OK) {
		printf("Open failed: %s\n", xnGetStatusString(nRetVal));
		xnEnumerationErrorsFree(pErrors);
		return (nRetVal);
	}

	xnEnumerationErrorsFree(pErrors);

	nRetVal = xnFindExistingRefNodeByType(pContext, XN_NODE_TYPE_DEPTH, &hDepth);
	CHECK_RC(nRetVal, "Find depth generator");

	pDepthMD = xnAllocateDepthMetaData();

	/* get parameters */
	matrix_t *matrix;
	int *next_frame;
	thread_info_t *thread_info;

	thread_info = (thread_info_t *) arg;
	matrix = thread_info->matrix;
	next_frame = thread_info->next_frame;

	while (*(next_frame) != THREAD_EXIT) {

		while (*(next_frame) != 1) {
			usleep(100);
		}

		nRetVal = xnWaitOneUpdateAll(pContext, hDepth);
		
		if (nRetVal != XN_STATUS_OK) {
			printf("UpdateData failed: %s\n", xnGetStatusString(nRetVal));
			continue;
		}

		xnGetDepthMetaData(hDepth, pDepthMD);
		pDepthMap = pDepthMD->pData;

		/* todo: COPY MATRIX */

		*next_frame = 0;
	}

	xnFreeDepthMetaData(pDepthMD);

	xnProductionNodeRelease(hDepth);
	xnProductionNodeRelease(hScriptNode);
	xnContextRelease(pContext);

	return NULL;
}
