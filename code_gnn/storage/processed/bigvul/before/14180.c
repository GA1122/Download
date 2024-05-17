int __glXDisp_WaitGL(__GLXclientState *cl, GLbyte *pc)
{
    xGLXWaitGLReq *req = (xGLXWaitGLReq *)pc;
    GLXContextTag tag = req->contextTag;
    __GLXcontext *glxc = NULL;
    int error;

    if (tag) {
	glxc = __glXLookupContextByTag(cl, tag);
	if (!glxc)
	    return __glXError(GLXBadContextTag);
    
	if (!__glXForceCurrent(cl, req->contextTag, &error))
	    return error;

	CALL_Finish( GET_DISPATCH(), () );
    }

    if (glxc && glxc->drawPriv->waitGL)
	(*glxc->drawPriv->waitGL)(glxc->drawPriv);

    return Success;
}
