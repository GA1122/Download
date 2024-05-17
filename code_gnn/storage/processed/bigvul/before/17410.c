SProcScreenSaverQueryInfo(ClientPtr client)
{
    REQUEST(xScreenSaverQueryInfoReq);
    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xScreenSaverQueryInfoReq);
    swapl(&stuff->drawable);
    return ProcScreenSaverQueryInfo(client);
}
