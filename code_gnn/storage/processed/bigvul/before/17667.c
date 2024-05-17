SProcXFixesShowCursor(ClientPtr client)
{
    REQUEST(xXFixesShowCursorReq);

    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xXFixesShowCursorReq);
    swapl(&stuff->window);
    return (*ProcXFixesVector[stuff->xfixesReqType]) (client);
}
