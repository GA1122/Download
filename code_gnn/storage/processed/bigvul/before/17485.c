SProcXvListImageFormats(ClientPtr client)
{
    REQUEST(xvListImageFormatsReq);
    REQUEST_SIZE_MATCH(xvListImageFormatsReq);
    swaps(&stuff->length);
    swapl(&stuff->port);
    return XvProcVector[xv_ListImageFormats] (client);
}
