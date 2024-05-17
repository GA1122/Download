ProcPanoramiXShmGetImage(ClientPtr client)
{
    PanoramiXRes *draw;
    DrawablePtr *drawables;
    DrawablePtr pDraw;
    xShmGetImageReply xgi;
    ShmDescPtr shmdesc;
    int i, x, y, w, h, format, rc;
    Mask plane = 0, planemask;
    long lenPer = 0, length, widthBytesLine;
    Bool isRoot;

    REQUEST(xShmGetImageReq);

    REQUEST_SIZE_MATCH(xShmGetImageReq);

    if ((stuff->format != XYPixmap) && (stuff->format != ZPixmap)) {
        client->errorValue = stuff->format;
        return BadValue;
    }

    rc = dixLookupResourceByClass((void **) &draw, stuff->drawable,
                                  XRC_DRAWABLE, client, DixWriteAccess);
    if (rc != Success)
        return (rc == BadValue) ? BadDrawable : rc;

    if (draw->type == XRT_PIXMAP)
        return ProcShmGetImage(client);

    rc = dixLookupDrawable(&pDraw, stuff->drawable, client, 0, DixReadAccess);
    if (rc != Success)
        return rc;

    VERIFY_SHMPTR(stuff->shmseg, stuff->offset, TRUE, shmdesc, client);

    x = stuff->x;
    y = stuff->y;
    w = stuff->width;
    h = stuff->height;
    format = stuff->format;
    planemask = stuff->planeMask;

    isRoot = (draw->type == XRT_WINDOW) && draw->u.win.root;

    if (isRoot) {
        if (                     
               x < 0 || x + w > PanoramiXPixWidth ||
               y < 0 || y + h > PanoramiXPixHeight)
            return BadMatch;
    }
    else {
        if (                     
               screenInfo.screens[0]->x + pDraw->x + x < 0 ||
               screenInfo.screens[0]->x + pDraw->x + x + w > PanoramiXPixWidth
               || screenInfo.screens[0]->y + pDraw->y + y < 0 ||
               screenInfo.screens[0]->y + pDraw->y + y + h > PanoramiXPixHeight
               ||
                
               x < -wBorderWidth((WindowPtr) pDraw) ||
               x + w > wBorderWidth((WindowPtr) pDraw) + (int) pDraw->width ||
               y < -wBorderWidth((WindowPtr) pDraw) ||
               y + h > wBorderWidth((WindowPtr) pDraw) + (int) pDraw->height)
            return BadMatch;
    }

    drawables = calloc(PanoramiXNumScreens, sizeof(DrawablePtr));
    if (!drawables)
        return BadAlloc;

    drawables[0] = pDraw;
    FOR_NSCREENS_FORWARD_SKIP(i) {
        rc = dixLookupDrawable(drawables + i, draw->info[i].id, client, 0,
                               DixReadAccess);
        if (rc != Success) {
            free(drawables);
            return rc;
        }
    }

    xgi = (xShmGetImageReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0,
        .visual = wVisual(((WindowPtr) pDraw)),
        .depth = pDraw->depth
    };

    if (format == ZPixmap) {
        widthBytesLine = PixmapBytePad(w, pDraw->depth);
        length = widthBytesLine * h;
    }
    else {
        widthBytesLine = PixmapBytePad(w, 1);
        lenPer = widthBytesLine * h;
        plane = ((Mask) 1) << (pDraw->depth - 1);
        length = lenPer * Ones(planemask & (plane | (plane - 1)));
    }

    VERIFY_SHMSIZE(shmdesc, stuff->offset, length, client);
    xgi.size = length;

    if (length == 0) {           
    }
    else if (format == ZPixmap) {
        XineramaGetImageData(drawables, x, y, w, h, format, planemask,
                             shmdesc->addr + stuff->offset,
                             widthBytesLine, isRoot);
    }
    else {

        length = stuff->offset;
        for (; plane; plane >>= 1) {
            if (planemask & plane) {
                XineramaGetImageData(drawables, x, y, w, h,
                                     format, plane, shmdesc->addr + length,
                                     widthBytesLine, isRoot);
                length += lenPer;
            }
        }
    }
    free(drawables);

    if (client->swapped) {
        swaps(&xgi.sequenceNumber);
        swapl(&xgi.length);
        swapl(&xgi.visual);
        swapl(&xgi.size);
    }
    WriteToClient(client, sizeof(xShmGetImageReply), &xgi);

    return Success;
}
