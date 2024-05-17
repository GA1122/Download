SProcRenderSetPictureFilter(ClientPtr client)
{
    REQUEST(xRenderSetPictureFilterReq);
    REQUEST_AT_LEAST_SIZE(xRenderSetPictureFilterReq);

    swaps(&stuff->length);
    swapl(&stuff->picture);
    swaps(&stuff->nbytes);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
