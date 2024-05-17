XvMCSurfaceInfo * XvMCListSurfaceTypes(Display *dpy, XvPortID port, int *num)
{
    XExtDisplayInfo *info = xvmc_find_display(dpy);
    xvmcListSurfaceTypesReply rep;
    xvmcListSurfaceTypesReq  *req;
    XvMCSurfaceInfo *surface_info = NULL;

    *num = 0;

    XvMCCheckExtension (dpy, info, NULL);

    LockDisplay (dpy);
    XvMCGetReq (ListSurfaceTypes, req);
    req->port = port;
    if (!_XReply (dpy, (xReply *) &rep, 0, xFalse)) {
        UnlockDisplay (dpy);
        SyncHandle ();
        return NULL;
    }

    if(rep.num > 0) {
        if (rep.num < (INT_MAX / sizeof(XvMCSurfaceInfo)))
            surface_info = Xmalloc(rep.num * sizeof(XvMCSurfaceInfo));

        if(surface_info) {
	    xvmcSurfaceInfo sinfo;
	    int i;

	    *num = rep.num;

	    for(i = 0; i < rep.num; i++) {
		_XRead(dpy, (char*)&sinfo, sizeof(xvmcSurfaceInfo));
	       surface_info[i].surface_type_id = sinfo.surface_type_id;
	       surface_info[i].chroma_format = sinfo.chroma_format;
	       surface_info[i].max_width = sinfo.max_width;
	       surface_info[i].max_height = sinfo.max_height;
	       surface_info[i].subpicture_max_width =
					sinfo.subpicture_max_width;
	       surface_info[i].subpicture_max_height =
					sinfo.subpicture_max_height;
	       surface_info[i].mc_type = sinfo.mc_type;
	       surface_info[i].flags = sinfo.flags;
	    }
	} else
	   _XEatDataWords(dpy, rep.length);
    }

    UnlockDisplay (dpy);
    SyncHandle ();
    return surface_info;
}
