SProcXvGetStill(ClientPtr client)
{
    REQUEST(xvGetStillReq);
    REQUEST_SIZE_MATCH(xvGetStillReq);
    swaps(&stuff->length);
    swapl(&stuff->port);
    swapl(&stuff->drawable);
    swapl(&stuff->gc);
    swaps(&stuff->vid_x);
    swaps(&stuff->vid_y);
    swaps(&stuff->vid_w);
    swaps(&stuff->vid_h);
    swaps(&stuff->drw_x);
    swaps(&stuff->drw_y);
    swaps(&stuff->drw_w);
    swaps(&stuff->drw_h);
    return XvProcVector[xv_GetStill] (client);
}
