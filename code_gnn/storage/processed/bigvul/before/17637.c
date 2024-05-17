SProcRenderTriStrip(ClientPtr client)
{
    REQUEST(xRenderTriStripReq);

    REQUEST_AT_LEAST_SIZE(xRenderTriStripReq);
    swaps(&stuff->length);
    swapl(&stuff->src);
    swapl(&stuff->dst);
    swapl(&stuff->maskFormat);
    swaps(&stuff->xSrc);
    swaps(&stuff->ySrc);
    SwapRestL(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
