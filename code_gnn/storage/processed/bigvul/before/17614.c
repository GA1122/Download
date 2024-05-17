SProcRenderCreateCursor(ClientPtr client)
{
    REQUEST(xRenderCreateCursorReq);
    REQUEST_SIZE_MATCH(xRenderCreateCursorReq);

    swaps(&stuff->length);
    swapl(&stuff->cid);
    swapl(&stuff->src);
    swaps(&stuff->x);
    swaps(&stuff->y);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
