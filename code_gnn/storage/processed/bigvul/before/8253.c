Status _xvmc_create_context (
    Display *dpy,
    XvMCContext *context,
    int *priv_count,
    CARD32 **priv_data
)
{
    XExtDisplayInfo *info = xvmc_find_display(dpy);
    xvmcCreateContextReply rep;
    xvmcCreateContextReq  *req;

    *priv_count = 0;
    *priv_data = NULL;

    XvMCCheckExtension (dpy, info, BadImplementation);

    LockDisplay (dpy);
    XvMCGetReq (CreateContext, req);
    context->context_id = XAllocID(dpy);
    req->context_id = context->context_id;
    req->port = context->port;
    req->surface_type_id = context->surface_type_id;
    req->width = context->width;
    req->height = context->height;
    req->flags = context->flags;
    if (!_XReply (dpy, (xReply *) &rep, 0, xFalse)) {
        UnlockDisplay (dpy);
        SyncHandle ();
        return BadImplementation;
    }
    context->width = rep.width_actual;
    context->height = rep.height_actual;
    context->flags = rep.flags_return;

    if(rep.length) {
	if (rep.length < (INT_MAX >> 2))
	    *priv_data = Xmalloc(rep.length << 2);
	if(*priv_data) {
            _XRead(dpy, (char*)(*priv_data), rep.length << 2);
	    *priv_count = rep.length;
	} else
	    _XEatDataWords(dpy, rep.length);
    }

    UnlockDisplay (dpy);
    SyncHandle ();
    return Success;
}
