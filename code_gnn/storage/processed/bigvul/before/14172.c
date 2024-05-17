int __glXDisp_QueryServerString(__GLXclientState *cl, GLbyte *pc)
{
    ClientPtr client = cl->client;
    xGLXQueryServerStringReq *req = (xGLXQueryServerStringReq *) pc;
    xGLXQueryServerStringReply reply;
    size_t n, length;
    const char *ptr;
    char *buf;
    __GLXscreen *pGlxScreen;
    int err;
    char ver_str[16];

    if (!validGlxScreen(client, req->screen, &pGlxScreen, &err))
	return err;

    switch(req->name) {
	case GLX_VENDOR:
	    ptr = pGlxScreen->GLXvendor;
	    break;
	case GLX_VERSION:
	     
	    snprintf(ver_str, 16, "%d.%d", glxMajorVersion, glxMinorVersion);
	    ptr = ver_str;
	    break;
	case GLX_EXTENSIONS:
	    ptr = pGlxScreen->GLXextensions;
	    break;
	default:
	    return BadValue; 
    }

    n = strlen(ptr) + 1;
    length = __GLX_PAD(n) >> 2;
    reply.type = X_Reply;
    reply.sequenceNumber = client->sequence;
    reply.length = length;
    reply.n = n;

    buf = (char *) malloc(length << 2);
    if (buf == NULL) {
        return BadAlloc;
    }
    memcpy(buf, ptr, n);

    if (client->swapped) {
        glxSwapQueryServerStringReply(client, &reply, buf);
    } else {
        WriteToClient(client, sz_xGLXQueryServerStringReply, (char *)&reply);
        WriteToClient(client, (int)(length << 2), buf);
    }

    free(buf);
    return Success;
}
