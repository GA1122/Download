SProcRenderFillRectangles(ClientPtr client)
{
    REQUEST(xRenderFillRectanglesReq);

    REQUEST_AT_LEAST_SIZE(xRenderFillRectanglesReq);
    swaps(&stuff->length);
    swapl(&stuff->dst);
    swaps(&stuff->color.red);
    swaps(&stuff->color.green);
    swaps(&stuff->color.blue);
    swaps(&stuff->color.alpha);
    SwapRestS(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
