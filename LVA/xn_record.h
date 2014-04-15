#ifndef _XN_RECORD_H_
#define _XN_RECORD_H_

#include <XnOpenNI.h>

#define SAMPLE_XML_PATH_LOCAL "SamplesConfig.xml"

#define CHECK_RC(rc, what)	\
if (rc != XN_STATUS_OK)		\
{ 							\
	return rc;				\
} 

void *xngrab_video(void *);
XnBool fileExists(const char *fn);

#endif