DoGetImage(ClientPtr client, int format, Drawable drawable,
           int x, int y, int width, int height,
           Mask planemask)
{
    DrawablePtr pDraw, pBoundingDraw;
    int nlines, linesPerBuf, rc;
    int linesDone;

     
    int relx, rely;
    long widthBytesLine, length;
    Mask plane = 0;
    char *pBuf;
    xGetImageReply xgi;
    RegionPtr pVisibleRegion = NULL;

    if ((format != XYPixmap) && (format != ZPixmap)) {
        client->errorValue = format;
        return BadValue;
    }
    rc = dixLookupDrawable(&pDraw, drawable, client, 0, DixReadAccess);
    if (rc != Success)
        return rc;

    memset(&xgi, 0, sizeof(xGetImageReply));

    relx = x;
    rely = y;

    if (pDraw->type == DRAWABLE_WINDOW) {
        WindowPtr pWin = (WindowPtr) pDraw;

         
        if (!pWin->viewable)
            return BadMatch;

         
        if (x < -wBorderWidth(pWin) ||
            x + width > wBorderWidth(pWin) + (int) pDraw->width ||
            y < -wBorderWidth(pWin) ||
            y + height > wBorderWidth(pWin) + (int) pDraw->height)
            return BadMatch;

        relx += pDraw->x;
        rely += pDraw->y;

        if (pDraw->pScreen->GetWindowPixmap) {
            PixmapPtr pPix = (*pDraw->pScreen->GetWindowPixmap) (pWin);

            pBoundingDraw = &pPix->drawable;
#ifdef COMPOSITE
            relx -= pPix->screen_x;
            rely -= pPix->screen_y;
#endif
        }
        else {
            pBoundingDraw = (DrawablePtr) pDraw->pScreen->root;
        }

        xgi.visual = wVisual(pWin);
    }
    else {
        pBoundingDraw = pDraw;
        xgi.visual = None;
    }

     
    if (relx < 0 || relx + width > (int) pBoundingDraw->width ||
        rely < 0 || rely + height > (int) pBoundingDraw->height)
        return BadMatch;

    xgi.type = X_Reply;
    xgi.sequenceNumber = client->sequence;
    xgi.depth = pDraw->depth;
    if (format == ZPixmap) {
        widthBytesLine = PixmapBytePad(width, pDraw->depth);
        length = widthBytesLine * height;

    }
    else {
        widthBytesLine = BitmapBytePad(width);
        plane = ((Mask) 1) << (pDraw->depth - 1);
         
        length = widthBytesLine * height *
            Ones(planemask & (plane | (plane - 1)));

    }

    xgi.length = length;

    xgi.length = bytes_to_int32(xgi.length);
    if (widthBytesLine == 0 || height == 0)
        linesPerBuf = 0;
    else if (widthBytesLine >= IMAGE_BUFSIZE)
        linesPerBuf = 1;
    else {
        linesPerBuf = IMAGE_BUFSIZE / widthBytesLine;
        if (linesPerBuf > height)
            linesPerBuf = height;
    }
    length = linesPerBuf * widthBytesLine;
    if (linesPerBuf < height) {
         
        while ((linesPerBuf > 1) &&
               (length & ((1L << LOG2_BYTES_PER_SCANLINE_PAD) - 1))) {
            linesPerBuf--;
            length -= widthBytesLine;
        }
        while (length & ((1L << LOG2_BYTES_PER_SCANLINE_PAD) - 1)) {
            linesPerBuf++;
            length += widthBytesLine;
        }
    }
    if (!(pBuf = calloc(1, length)))
        return BadAlloc;
    WriteReplyToClient(client, sizeof(xGetImageReply), &xgi);

    if (pDraw->type == DRAWABLE_WINDOW) {
        pVisibleRegion = NotClippedByChildren((WindowPtr) pDraw);
        if (pVisibleRegion) {
            RegionTranslate(pVisibleRegion, -pDraw->x, -pDraw->y);
        }
    }

    if (linesPerBuf == 0) {
         
    }
    else if (format == ZPixmap) {
        linesDone = 0;
        while (height - linesDone > 0) {
            nlines = min(linesPerBuf, height - linesDone);
            (*pDraw->pScreen->GetImage) (pDraw,
                                         x,
                                         y + linesDone,
                                         width,
                                         nlines,
                                         format, planemask, (void *) pBuf);
            if (pVisibleRegion)
                XaceCensorImage(client, pVisibleRegion, widthBytesLine,
                                pDraw, x, y + linesDone, width,
                                nlines, format, pBuf);

             
            ReformatImage(pBuf, (int) (nlines * widthBytesLine),
                          BitsPerPixel(pDraw->depth), ClientOrder(client));

            WriteToClient(client, (int) (nlines * widthBytesLine), pBuf);
            linesDone += nlines;
        }
    }
    else {                       

        for (; plane; plane >>= 1) {
            if (planemask & plane) {
                linesDone = 0;
                while (height - linesDone > 0) {
                    nlines = min(linesPerBuf, height - linesDone);
                    (*pDraw->pScreen->GetImage) (pDraw,
                                                 x,
                                                 y + linesDone,
                                                 width,
                                                 nlines,
                                                 format, plane, (void *) pBuf);
                    if (pVisibleRegion)
                        XaceCensorImage(client, pVisibleRegion,
                                        widthBytesLine,
                                        pDraw, x, y + linesDone, width,
                                        nlines, format, pBuf);

                     
                    ReformatImage(pBuf, (int) (nlines * widthBytesLine),
                                  1, ClientOrder(client));

                    WriteToClient(client, (int)(nlines * widthBytesLine), pBuf);
                    linesDone += nlines;
                }
            }
        }
    }
    if (pVisibleRegion)
        RegionDestroy(pVisibleRegion);
    free(pBuf);
    return Success;
}
