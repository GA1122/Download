ProcPanoramiXShmPutImage(ClientPtr client)
{
    int j, result, orig_x, orig_y;
    PanoramiXRes *draw, *gc;
    Bool sendEvent, isRoot;

    REQUEST(xShmPutImageReq);
    REQUEST_SIZE_MATCH(xShmPutImageReq);

    result = dixLookupResourceByClass((void **) &draw, stuff->drawable,
                                      XRC_DRAWABLE, client, DixWriteAccess);
    if (result != Success)
        return (result == BadValue) ? BadDrawable : result;

    result = dixLookupResourceByType((void **) &gc, stuff->gc,
                                     XRT_GC, client, DixReadAccess);
    if (result != Success)
        return result;

    isRoot = (draw->type == XRT_WINDOW) && draw->u.win.root;

    orig_x = stuff->dstX;
    orig_y = stuff->dstY;
    sendEvent = stuff->sendEvent;
    stuff->sendEvent = 0;
    FOR_NSCREENS(j) {
        if (!j)
            stuff->sendEvent = sendEvent;
        stuff->drawable = draw->info[j].id;
        stuff->gc = gc->info[j].id;
        if (isRoot) {
            stuff->dstX = orig_x - screenInfo.screens[j]->x;
            stuff->dstY = orig_y - screenInfo.screens[j]->y;
        }
        result = ProcShmPutImage(client);
        if (result != Success)
            break;
    }
    return result;
}
