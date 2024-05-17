ProcUnmapWindow(ClientPtr client)
{
    WindowPtr pWin;

    REQUEST(xResourceReq);
    int rc;

    REQUEST_SIZE_MATCH(xResourceReq);
    rc = dixLookupWindow(&pWin, stuff->id, client, DixHideAccess);
    if (rc != Success)
        return rc;
    UnmapWindow(pWin, FALSE);
     
    return Success;
}
