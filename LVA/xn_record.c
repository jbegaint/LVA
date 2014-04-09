#include <XnOpenNI.h>

#define SAMPLE_XML_PATH "/usr/share/openni/SamplesConfig.xml"
#define SAMPLE_XML_PATH_LOCAL "SamplesConfig.xml"

#define CHECK_RC(rc, what)											\
	if (rc != XN_STATUS_OK)											\
	{																\
		printf("%s failed: %s\n", what, xnGetStatusString(rc));		\
		return rc;													\
	}

XnBool fileExists(const char *fn)
{
	XnBool exists;
	xnOSDoesFileExist(fn, &exists);
	return exists;
}

void xngrab_video(void *arg)
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

	while (!xnOSWasKeyboardHit()) {

		nRetVal = xnWaitOneUpdateAll(pContext, hDepth);
		
		if (nRetVal != XN_STATUS_OK) {
			printf("UpdateData failed: %s\n", xnGetStatusString(nRetVal));
			continue;
		}

		xnGetDepthMetaData(hDepth, pDepthMD);
		pDepthMap = pDepthMD->pData;
		middlePoint = pDepthMap[pDepthMD->pMap->Res.X * pDepthMD->pMap->Res.Y / 2 
						+ pDepthMD->pMap->Res.X / 2];

		printf("Frame %d Middle point is: %u\n", pDepthMD->pMap->pOutput->nFrameID,
			   middlePoint);
	}

	xnFreeDepthMetaData(pDepthMD);

	xnProductionNodeRelease(hDepth);
	xnProductionNodeRelease(hScriptNode);
	xnContextRelease(pContext);

	return NULL;
}
