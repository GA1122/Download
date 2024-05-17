SProcRenderTriangles(ClientPtr client)
{
    REQUEST(xRenderTrianglesReq);

    REQUEST_AT_LEAST_SIZE(xRenderTrianglesReq);
    swaps(&stuff->length);
    swapl(&stuff->src);
    swapl(&stuff->dst);
    swapl(&stuff->maskFormat);
    swaps(&stuff->xSrc);
    swaps(&stuff->ySrc);
    SwapRestL(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
