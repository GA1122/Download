int __glXDisp_QueryVersion(__GLXclientState *cl, GLbyte *pc)
{
    ClientPtr client = cl->client;
    xGLXQueryVersionReq *req = (xGLXQueryVersionReq *) pc;
    xGLXQueryVersionReply reply;
    GLuint major, minor;

    major = req->majorVersion;
    minor = req->minorVersion;
    (void)major;
    (void)minor;

     
    reply.majorVersion = glxMajorVersion;
    reply.minorVersion = glxMinorVersion;
    reply.length = 0;
    reply.type = X_Reply;
    reply.sequenceNumber = client->sequence;

    if (client->swapped) {
	__glXSwapQueryVersionReply(client, &reply);
    } else {
	WriteToClient(client, sz_xGLXQueryVersionReply, (char *)&reply);
    }
    return Success;
}
