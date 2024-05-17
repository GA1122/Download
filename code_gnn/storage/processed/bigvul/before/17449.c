SProcXResQueryClientPixmapBytes(ClientPtr client)
{
    REQUEST(xXResQueryClientPixmapBytesReq);
    REQUEST_SIZE_MATCH(xXResQueryClientPixmapBytesReq);
    swapl(&stuff->xid);
    return ProcXResQueryClientPixmapBytes(client);
}
