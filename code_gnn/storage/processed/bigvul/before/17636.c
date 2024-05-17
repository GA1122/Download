SProcRenderTriFan(ClientPtr client)
{
    REQUEST(xRenderTriFanReq);

    REQUEST_AT_LEAST_SIZE(xRenderTriFanReq);
    swaps(&stuff->length);
    swapl(&stuff->src);
    swapl(&stuff->dst);
    swapl(&stuff->maskFormat);
    swaps(&stuff->xSrc);
    swaps(&stuff->ySrc);
    SwapRestL(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
