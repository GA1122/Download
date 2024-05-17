SProcRenderAddTraps(ClientPtr client)
{
    REQUEST(xRenderAddTrapsReq);
    REQUEST_AT_LEAST_SIZE(xRenderAddTrapsReq);

    swaps(&stuff->length);
    swapl(&stuff->picture);
    swaps(&stuff->xOff);
    swaps(&stuff->yOff);
    SwapRestL(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
