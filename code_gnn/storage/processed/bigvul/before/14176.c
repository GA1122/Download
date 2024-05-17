int __glXDisp_RenderLarge(__GLXclientState *cl, GLbyte *pc)
{
    xGLXRenderLargeReq *req;
    ClientPtr client= cl->client;
    size_t dataBytes;
    __GLXrenderLargeHeader *hdr;
    __GLXcontext *glxc;
    int error;
    CARD16 opcode;
    __GLX_DECLARE_SWAP_VARIABLES;
    
    req = (xGLXRenderLargeReq *) pc;
    if (client->swapped) {
	__GLX_SWAP_SHORT(&req->length);
	__GLX_SWAP_INT(&req->contextTag);
	__GLX_SWAP_INT(&req->dataBytes);
	__GLX_SWAP_SHORT(&req->requestNumber);
	__GLX_SWAP_SHORT(&req->requestTotal);
    }

    glxc = __glXForceCurrent(cl, req->contextTag, &error);
    if (!glxc) {
	 
	__glXResetLargeCommandStatus(cl);
	return error;
    }
    dataBytes = req->dataBytes;

     
    if ((req->length << 2) != __GLX_PAD(dataBytes) + sz_xGLXRenderLargeReq) {
	client->errorValue = req->length;
	 
	__glXResetLargeCommandStatus(cl);
	return BadLength;
    }
    pc += sz_xGLXRenderLargeReq;
    
    if (cl->largeCmdRequestsSoFar == 0) {
	__GLXrenderSizeData entry;
	int extra;
	size_t cmdlen;
	int err;

	 
	if (req->requestNumber != 1) {
	    client->errorValue = req->requestNumber;
	    return __glXError(GLXBadLargeRequest);
	}

	hdr = (__GLXrenderLargeHeader *) pc;
	if (client->swapped) {
	    __GLX_SWAP_INT(&hdr->length);
	    __GLX_SWAP_INT(&hdr->opcode);
	}
	cmdlen = hdr->length;
	opcode = hdr->opcode;

	 
	err = __glXGetProtocolSizeData(& Render_dispatch_info, opcode, & entry);
	if (err < 0) {
	    client->errorValue = opcode;
	    return __glXError(GLXBadLargeRequest);
	}

	if (entry.varsize) {
	     
	    extra = (*entry.varsize)(pc + __GLX_RENDER_LARGE_HDR_SIZE,
				     client->swapped);
	    if (extra < 0) {
		extra = 0;
	    }
	     
	    if (cmdlen != __GLX_PAD(entry.bytes + 4 + extra)) {
		return BadLength;
	    }
	} else {
	     
	    if (cmdlen != __GLX_PAD(entry.bytes + 4)) {
		return BadLength;
	    }
	}
	 
	if (cl->largeCmdBufSize < cmdlen) {
	    if (!cl->largeCmdBuf) {
		cl->largeCmdBuf = (GLbyte *) malloc(cmdlen);
	    } else {
		cl->largeCmdBuf = (GLbyte *) realloc(cl->largeCmdBuf, cmdlen);
	    }
	    if (!cl->largeCmdBuf) {
		return BadAlloc;
	    }
	    cl->largeCmdBufSize = cmdlen;
	}
	memcpy(cl->largeCmdBuf, pc, dataBytes);

	cl->largeCmdBytesSoFar = dataBytes;
	cl->largeCmdBytesTotal = cmdlen;
	cl->largeCmdRequestsSoFar = 1;
	cl->largeCmdRequestsTotal = req->requestTotal;
	return Success;
	
    } else {
	 

	 
	if (req->requestNumber != cl->largeCmdRequestsSoFar + 1) {
	    client->errorValue = req->requestNumber;
	    __glXResetLargeCommandStatus(cl);
	    return __glXError(GLXBadLargeRequest);
	}
	if (req->requestTotal != cl->largeCmdRequestsTotal) {
	    client->errorValue = req->requestTotal;
	    __glXResetLargeCommandStatus(cl);
	    return __glXError(GLXBadLargeRequest);
	}

	 
	if ((cl->largeCmdBytesSoFar + dataBytes) > cl->largeCmdBytesTotal) {
	    client->errorValue = dataBytes;
	    __glXResetLargeCommandStatus(cl);
	    return __glXError(GLXBadLargeRequest);
	}
	memcpy(cl->largeCmdBuf + cl->largeCmdBytesSoFar, pc, dataBytes);
	cl->largeCmdBytesSoFar += dataBytes;
	cl->largeCmdRequestsSoFar++;

	if (req->requestNumber == cl->largeCmdRequestsTotal) {
	    __GLXdispatchRenderProcPtr proc;

	     
	     
	    if (__GLX_PAD(cl->largeCmdBytesSoFar) !=
		__GLX_PAD(cl->largeCmdBytesTotal)) {
		client->errorValue = dataBytes;
		__glXResetLargeCommandStatus(cl);
		return __glXError(GLXBadLargeRequest);
	    }
	    hdr = (__GLXrenderLargeHeader *) cl->largeCmdBuf;
	     
	    opcode = hdr->opcode;

	    proc = (__GLXdispatchRenderProcPtr)
	      __glXGetProtocolDecodeFunction(& Render_dispatch_info, opcode,
					     client->swapped);
	    if (proc == NULL) {
		client->errorValue = opcode;
		return __glXError(GLXBadLargeRequest);
	    }

	     
	    (*proc)(cl->largeCmdBuf + __GLX_RENDER_LARGE_HDR_SIZE);
	    __GLX_NOTE_UNFLUSHED_CMDS(glxc);

	     
	    __glXResetLargeCommandStatus(cl);
	} else {
	     
	}
	return Success;
    }
}
