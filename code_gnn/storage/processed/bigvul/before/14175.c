int __glXDisp_Render(__GLXclientState *cl, GLbyte *pc)
{
    xGLXRenderReq *req;
    ClientPtr client= cl->client;
    int left, cmdlen, error;
    int commandsDone;
    CARD16 opcode;
    __GLXrenderHeader *hdr;
    __GLXcontext *glxc;
    __GLX_DECLARE_SWAP_VARIABLES;

    req = (xGLXRenderReq *) pc;
    if (client->swapped) {
	__GLX_SWAP_SHORT(&req->length);
	__GLX_SWAP_INT(&req->contextTag);
    }

    glxc = __glXForceCurrent(cl, req->contextTag, &error);
    if (!glxc) {
	return error;
    }

    commandsDone = 0;
    pc += sz_xGLXRenderReq;
    left = (req->length << 2) - sz_xGLXRenderReq;
    while (left > 0) {
        __GLXrenderSizeData entry;
        int extra;
	__GLXdispatchRenderProcPtr proc;
	int err;

	 
	hdr = (__GLXrenderHeader *) pc;
	if (client->swapped) {
	    __GLX_SWAP_SHORT(&hdr->length);
	    __GLX_SWAP_SHORT(&hdr->opcode);
	}
	cmdlen = hdr->length;
	opcode = hdr->opcode;

	 
	err = __glXGetProtocolSizeData(& Render_dispatch_info, opcode, & entry);
	proc = (__GLXdispatchRenderProcPtr)
	    __glXGetProtocolDecodeFunction(& Render_dispatch_info,
					   opcode, client->swapped);

	if ((err < 0) || (proc == NULL)) {
	    client->errorValue = commandsDone;
	    return __glXError(GLXBadRenderRequest);
	}

        if (entry.varsize) {
             
            extra = (*entry.varsize)(pc + __GLX_RENDER_HDR_SIZE,
				     client->swapped);
            if (extra < 0) {
                extra = 0;
            }
            if (cmdlen != __GLX_PAD(entry.bytes + extra)) {
                return BadLength;
            }
        } else {
             
            if (cmdlen != __GLX_PAD(entry.bytes)) {
                return BadLength;
            }
        }
	if (left < cmdlen) {
	    return BadLength;
	}

	 
	(*proc)(pc + __GLX_RENDER_HDR_SIZE);
	pc += cmdlen;
	left -= cmdlen;
	commandsDone++;
    }
    __GLX_NOTE_UNFLUSHED_CMDS(glxc);
    return Success;
}
