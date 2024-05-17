SProcXvGrabPort(ClientPtr client)
{
    REQUEST(xvGrabPortReq);
    REQUEST_SIZE_MATCH(xvGrabPortReq);
    swaps(&stuff->length);
    swapl(&stuff->port);
    swapl(&stuff->time);
    return XvProcVector[xv_GrabPort] (client);
}
