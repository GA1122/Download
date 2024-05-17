SProcRenderTrapezoids(ClientPtr client)
{
    REQUEST(xRenderTrapezoidsReq);

    REQUEST_AT_LEAST_SIZE(xRenderTrapezoidsReq);
    swaps(&stuff->length);
    swapl(&stuff->src);
    swapl(&stuff->dst);
    swapl(&stuff->maskFormat);
    swaps(&stuff->xSrc);
    swaps(&stuff->ySrc);
    SwapRestL(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
