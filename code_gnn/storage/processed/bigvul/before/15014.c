ProcSetDashes(ClientPtr client)
{
    GC *pGC;
    int result;

    REQUEST(xSetDashesReq);

    REQUEST_FIXED_SIZE(xSetDashesReq, stuff->nDashes);
    if (stuff->nDashes == 0) {
        client->errorValue = 0;
        return BadValue;
    }

    result = dixLookupGC(&pGC, stuff->gc, client, DixSetAttrAccess);
    if (result != Success)
        return result;

     
    client->errorValue = 0;
    return SetDashes(pGC, stuff->dashOffset, stuff->nDashes,
                     (unsigned char *) &stuff[1]);
}
