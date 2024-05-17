SProcRenderSetPictureClipRectangles(ClientPtr client)
{
    REQUEST(xRenderSetPictureClipRectanglesReq);
    REQUEST_AT_LEAST_SIZE(xRenderSetPictureClipRectanglesReq);
    swaps(&stuff->length);
    swapl(&stuff->picture);
    swaps(&stuff->xOrigin);
    swaps(&stuff->yOrigin);
    SwapRestS(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
