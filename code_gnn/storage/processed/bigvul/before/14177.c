int __glXDisp_SwapBuffers(__GLXclientState *cl, GLbyte *pc)
{
    ClientPtr client = cl->client;
    xGLXSwapBuffersReq *req = (xGLXSwapBuffersReq *) pc;
    GLXContextTag tag = req->contextTag;
    XID drawId = req->drawable;
    __GLXcontext *glxc = NULL;
    __GLXdrawable *pGlxDraw;
    int error;

    if (tag) {
	glxc = __glXLookupContextByTag(cl, tag);
	if (!glxc) {
	    return __glXError(GLXBadContextTag);
	}
	 
	if (__glXForceCurrent(cl, tag, &error)) {
	     
	    CALL_Finish( GET_DISPATCH(), () );
	    __GLX_NOTE_FLUSHED_CMDS(glxc);
	} else {
	    return error;
	}
    }

    pGlxDraw = __glXGetDrawable(glxc, drawId, client, &error);
    if (pGlxDraw == NULL)
	return error;

    if (pGlxDraw->type == DRAWABLE_WINDOW &&
	(*pGlxDraw->swapBuffers)(cl->client, pGlxDraw) == GL_FALSE)
	return __glXError(GLXBadDrawable);

    return Success;
}
