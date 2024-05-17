int __glXDisp_CopyContext(__GLXclientState *cl, GLbyte *pc)
{
    ClientPtr client = cl->client;
    xGLXCopyContextReq *req = (xGLXCopyContextReq *) pc;
    GLXContextID source = req->source;
    GLXContextID dest = req->dest;
    GLXContextTag tag = req->contextTag;
    unsigned long mask = req->mask;
    __GLXcontext *src, *dst;
    int error;

    if (!validGlxContext(cl->client, source, DixReadAccess, &src, &error))
	return error;
    if (!validGlxContext(cl->client, dest, DixWriteAccess, &dst, &error))
	return error;

     
    if (src->isDirect || dst->isDirect ||
	(src->pGlxScreen != dst->pGlxScreen)) {
	client->errorValue = source;
	return BadMatch;
    }

     
    if (dst->isCurrent) {
	client->errorValue = dest;
	return BadAccess;
    }

    if (tag) {
	__GLXcontext *tagcx = __glXLookupContextByTag(cl, tag);
	
	if (!tagcx) {
	    return __glXError(GLXBadContextTag);
	}
	if (tagcx != src) {
	     
	    return BadMatch;
	}
	 
	if (__glXForceCurrent(cl, tag, &error)) {
	     
	    CALL_Finish( GET_DISPATCH(), () );
	    __GLX_NOTE_FLUSHED_CMDS(tagcx);
	} else {
	    return error;
	}
    }
     
    if (!(*dst->copy)(dst, src, mask)) {
	client->errorValue = mask;
	return BadValue;
    }
    return Success;
}
