SProcXvSelectVideoNotify(ClientPtr client)
{
    REQUEST(xvSelectVideoNotifyReq);
    REQUEST_SIZE_MATCH(xvSelectVideoNotifyReq);
    swaps(&stuff->length);
    swapl(&stuff->drawable);
    return XvProcVector[xv_SelectVideoNotify] (client);
}
