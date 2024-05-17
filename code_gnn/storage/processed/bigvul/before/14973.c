ProcFreeColormap(ClientPtr client)
{
    ColormapPtr pmap;
    int rc;

    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    rc = dixLookupResourceByType((void **) &pmap, stuff->id, RT_COLORMAP,
                                 client, DixDestroyAccess);
    if (rc == Success) {
         
        if (!(pmap->flags & IsDefault))
            FreeResource(stuff->id, RT_NONE);
        return Success;
    }
    else {
        client->errorValue = stuff->id;
        return rc;
    }
}
