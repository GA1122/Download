http2shmlog(const struct http *hp, int t)
{

	CHECK_OBJ_NOTNULL(hp, HTTP_MAGIC);
	if (t > HTTP_HDR_FIRST)
		t = HTTP_HDR_FIRST;
	assert(hp->logtag >= HTTP_Rx && hp->logtag <= HTTP_Obj);  
	assert(t >= HTTP_HDR_REQ && t <= HTTP_HDR_FIRST);
	return (logmtx[hp->logtag][t]);
}