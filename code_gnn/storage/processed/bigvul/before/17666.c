SProcXFixesSetCursorName(ClientPtr client)
{
    REQUEST(xXFixesSetCursorNameReq);

    swaps(&stuff->length);
    REQUEST_AT_LEAST_SIZE(xXFixesSetCursorNameReq);
    swapl(&stuff->cursor);
    swaps(&stuff->nbytes);
    return (*ProcXFixesVector[stuff->xfixesReqType]) (client);
}
