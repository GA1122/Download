SProcXvSelectPortNotify(ClientPtr client)
{
    REQUEST(xvSelectPortNotifyReq);
    REQUEST_SIZE_MATCH(xvSelectPortNotifyReq);
    swaps(&stuff->length);
    swapl(&stuff->port);
    return XvProcVector[xv_SelectPortNotify] (client);
}
