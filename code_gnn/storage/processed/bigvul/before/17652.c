ProcXFixesGetCursorImage(ClientPtr client)
{
 
    xXFixesGetCursorImageReply *rep;
    CursorPtr pCursor;
    CARD32 *image;
    int npixels, width, height, rc, x, y;

    REQUEST_SIZE_MATCH(xXFixesGetCursorImageReq);
    pCursor = CursorCurrent[PickPointer(client)->id];
    if (!pCursor)
        return BadCursor;
    rc = XaceHook(XACE_RESOURCE_ACCESS, client, pCursor->id, RT_CURSOR,
                  pCursor, RT_NONE, NULL, DixReadAccess);
    if (rc != Success)
        return rc;
    GetSpritePosition(PickPointer(client), &x, &y);
    width = pCursor->bits->width;
    height = pCursor->bits->height;
    npixels = width * height;
    rep = calloc(sizeof(xXFixesGetCursorImageReply) + npixels * sizeof(CARD32),
                 1);
    if (!rep)
        return BadAlloc;

    rep->type = X_Reply;
    rep->sequenceNumber = client->sequence;
    rep->length = npixels;
    rep->width = width;
    rep->height = height;
    rep->x = x;
    rep->y = y;
    rep->xhot = pCursor->bits->xhot;
    rep->yhot = pCursor->bits->yhot;
    rep->cursorSerial = pCursor->serialNumber;

    image = (CARD32 *) (rep + 1);
    CopyCursorToImage(pCursor, image);
    if (client->swapped) {
        swaps(&rep->sequenceNumber);
        swapl(&rep->length);
        swaps(&rep->x);
        swaps(&rep->y);
        swaps(&rep->width);
        swaps(&rep->height);
        swaps(&rep->xhot);
        swaps(&rep->yhot);
        swapl(&rep->cursorSerial);
        SwapLongs(image, npixels);
    }
    WriteToClient(client,
                  sizeof(xXFixesGetCursorImageReply) + (npixels << 2), rep);
    free(rep);
    return Success;
}
