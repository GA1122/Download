SProcXvUngrabPort(ClientPtr client)
{
    REQUEST(xvUngrabPortReq);
    REQUEST_SIZE_MATCH(xvUngrabPortReq);
    swaps(&stuff->length);
    swapl(&stuff->port);
    swapl(&stuff->time);
    return XvProcVector[xv_UngrabPort] (client);
}
