SProcXvShmPutImage(ClientPtr client)
{
    REQUEST(xvShmPutImageReq);
    REQUEST_SIZE_MATCH(xvShmPutImageReq);
    swaps(&stuff->length);
    swapl(&stuff->port);
    swapl(&stuff->drawable);
    swapl(&stuff->gc);
    swapl(&stuff->shmseg);
    swapl(&stuff->id);
    swapl(&stuff->offset);
    swaps(&stuff->src_x);
    swaps(&stuff->src_y);
    swaps(&stuff->src_w);
    swaps(&stuff->src_h);
    swaps(&stuff->drw_x);
    swaps(&stuff->drw_y);
    swaps(&stuff->drw_w);
    swaps(&stuff->drw_h);
    swaps(&stuff->width);
    swaps(&stuff->height);
    return XvProcVector[xv_ShmPutImage] (client);
}
