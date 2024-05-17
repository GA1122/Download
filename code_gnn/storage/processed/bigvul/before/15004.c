ProcPolyRectangle(ClientPtr client)
{
    int nrects;
    GC *pGC;
    DrawablePtr pDraw;

    REQUEST(xPolyRectangleReq);

    REQUEST_AT_LEAST_SIZE(xPolyRectangleReq);
    VALIDATE_DRAWABLE_AND_GC(stuff->drawable, pDraw, DixWriteAccess);
    nrects = (client->req_len << 2) - sizeof(xPolyRectangleReq);
    if (nrects & 4)
        return BadLength;
    nrects >>= 3;
    if (nrects)
        (*pGC->ops->PolyRectangle) (pDraw, pGC,
                                    nrects, (xRectangle *) &stuff[1]);
    return Success;
}
