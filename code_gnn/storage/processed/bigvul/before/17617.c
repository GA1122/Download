SProcRenderCreatePicture(ClientPtr client)
{
    REQUEST(xRenderCreatePictureReq);
    REQUEST_AT_LEAST_SIZE(xRenderCreatePictureReq);
    swaps(&stuff->length);
    swapl(&stuff->pid);
    swapl(&stuff->drawable);
    swapl(&stuff->format);
    swapl(&stuff->mask);
    SwapRestL(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
