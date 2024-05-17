XRenderFindDisplay (Display *dpy)
{
    XRenderExtDisplayInfo *dpyinfo;

    dpyinfo = XRenderExtFindDisplay (&XRenderExtensionInfo, dpy);
    if (!dpyinfo)
	dpyinfo = XRenderExtAddDisplay (&XRenderExtensionInfo, dpy,
                                        XRenderExtensionName);
    return dpyinfo;
}
