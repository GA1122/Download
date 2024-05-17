ProcXFixesGetCursorImageAndName(ClientPtr client)
{
 
    xXFixesGetCursorImageAndNameReply *rep;
    CursorPtr pCursor;
    CARD32 *image;
    int npixels;
    const char *name;
    int nbytes, nbytesRound;
    int width, height;
    int rc, x, y;

    REQUEST_SIZE_MATCH(xXFixesGetCursorImageAndNameReq);
    pCursor = CursorCurrent[PickPointer(client)->id];
    if (!pCursor)
        return BadCursor;
    rc = XaceHook(XACE_RESOURCE_ACCESS, client, pCursor->id, RT_CURSOR,
                  pCursor, RT_NONE, NULL, DixReadAccess | DixGetAttrAccess);
    if (rc != Success)
        return rc;
    GetSpritePosition(PickPointer(client), &x, &y);
    width = pCursor->bits->width;
    height = pCursor->bits->height;
    npixels = width * height;
    name = pCursor->name ? NameForAtom(pCursor->name) : "";
    nbytes = strlen(name);
    nbytesRound = pad_to_int32(nbytes);
    rep = calloc(sizeof(xXFixesGetCursorImageAndNameReply) +
                 npixels * sizeof(CARD32) + nbytesRound, 1);
    if (!rep)
        return BadAlloc;

    rep->type = X_Reply;
    rep->sequenceNumber = client->sequence;
    rep->length = npixels + bytes_to_int32(nbytesRound);
    rep->width = width;
    rep->height = height;
    rep->x = x;
    rep->y = y;
    rep->xhot = pCursor->bits->xhot;
    rep->yhot = pCursor->bits->yhot;
    rep->cursorSerial = pCursor->serialNumber;
    rep->cursorName = pCursor->name;
    rep->nbytes = nbytes;

    image = (CARD32 *) (rep + 1);
    CopyCursorToImage(pCursor, image);
    memcpy((image + npixels), name, nbytes);
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
        swapl(&rep->cursorName);
        swaps(&rep->nbytes);
        SwapLongs(image, npixels);
    }
    WriteToClient(client, sizeof(xXFixesGetCursorImageAndNameReply) +
                  (npixels << 2) + nbytesRound, rep);
    free(rep);
    return Success;
}
