ProcDbeDeallocateBackBufferName(ClientPtr client)
{
    REQUEST(xDbeDeallocateBackBufferNameReq);
    DbeWindowPrivPtr pDbeWindowPriv;
    int rc, i;
    void *val;

    REQUEST_SIZE_MATCH(xDbeDeallocateBackBufferNameReq);

     
    rc = dixLookupResourceByType((void **) &pDbeWindowPriv, stuff->buffer,
                                 dbeWindowPrivResType, client,
                                 DixDestroyAccess);
    if (rc != Success)
        return rc;

    rc = dixLookupResourceByType(&val, stuff->buffer, dbeDrawableResType,
                                 client, DixDestroyAccess);
    if (rc != Success)
        return rc;

     

    for (i = 0; i < pDbeWindowPriv->nBufferIDs; i++) {
         
        if (pDbeWindowPriv->IDs[i] == stuff->buffer) {
            break;
        }
    }

    if (i == pDbeWindowPriv->nBufferIDs) {
         
        client->errorValue = stuff->buffer;
        return dbeErrorBase + DbeBadBuffer;
    }

    FreeResource(stuff->buffer, RT_NONE);

    return Success;

}                                
