SProcXvQueryBestSize(ClientPtr client)
{
    REQUEST(xvQueryBestSizeReq);
    REQUEST_SIZE_MATCH(xvQueryBestSizeReq);
    swaps(&stuff->length);
    swapl(&stuff->port);
    swaps(&stuff->vid_w);
    swaps(&stuff->vid_h);
    swaps(&stuff->drw_w);
    swaps(&stuff->drw_h);
    return XvProcVector[xv_QueryBestSize] (client);
}
