XRenderCloseDisplay (Display *dpy, XExtCodes *codes)
{
    XRenderExtDisplayInfo *info = XRenderFindDisplay (dpy);
    if (info && info->info) XFree (info->info);

    return XRenderExtRemoveDisplay (&XRenderExtensionInfo, dpy);
}
