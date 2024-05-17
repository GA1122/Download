SProcRenderChangePicture (ClientPtr client)
{
    register int n;
    REQUEST(xRenderChangePictureReq);
    swaps(&stuff->length, n);
    swapl(&stuff->picture, n);
    swapl(&stuff->mask, n);
    SwapRestL(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
