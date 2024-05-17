SProcXFixesGetCursorName(ClientPtr client)
{
    REQUEST(xXFixesGetCursorNameReq);

    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xXFixesGetCursorNameReq);
    swapl(&stuff->cursor);
    return (*ProcXFixesVector[stuff->xfixesReqType]) (client);
}
