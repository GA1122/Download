SProcRenderFillRectangles (ClientPtr client)
{
    register int n;
    REQUEST(xRenderFillRectanglesReq);

    REQUEST_AT_LEAST_SIZE (xRenderFillRectanglesReq);
    swaps(&stuff->length, n);
    swapl(&stuff->dst, n);
    swaps(&stuff->color.red, n);
    swaps(&stuff->color.green, n);
    swaps(&stuff->color.blue, n);
    swaps(&stuff->color.alpha, n);
    SwapRestS(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
