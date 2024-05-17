int __glXDisp_WaitX(__GLXclientState *cl, GLbyte *pc)
{
    xGLXWaitXReq *req = (xGLXWaitXReq *)pc;
    GLXContextTag tag = req->contextTag;
    __GLXcontext *glxc = NULL;
    int error;

    if (tag) {
	glxc = __glXLookupContextByTag(cl, tag);
	if (!glxc)
	    return __glXError(GLXBadContextTag);
    
	if (!__glXForceCurrent(cl, req->contextTag, &error))
	    return error;
    }

    if (glxc && glxc->drawPriv->waitX)
	(*glxc->drawPriv->waitX)(glxc->drawPriv);

    return Success;
}
