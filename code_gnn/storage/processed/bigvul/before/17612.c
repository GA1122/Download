SProcRenderCreateAnimCursor(ClientPtr client)
{
    REQUEST(xRenderCreateAnimCursorReq);
    REQUEST_AT_LEAST_SIZE(xRenderCreateAnimCursorReq);

    swaps(&stuff->length);
    swapl(&stuff->cid);
    SwapRestL(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
