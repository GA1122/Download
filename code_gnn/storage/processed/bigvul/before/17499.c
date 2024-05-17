SProcXvStopVideo(ClientPtr client)
{
    REQUEST(xvStopVideoReq);
    REQUEST_SIZE_MATCH(xvStopVideoReq);
    swaps(&stuff->length);
    swapl(&stuff->port);
    swapl(&stuff->drawable);
    return XvProcVector[xv_StopVideo] (client);
}
