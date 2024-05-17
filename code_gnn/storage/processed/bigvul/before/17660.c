SProcXFixesChangeCursor(ClientPtr client)
{
    REQUEST(xXFixesChangeCursorReq);

    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xXFixesChangeCursorReq);
    swapl(&stuff->source);
    swapl(&stuff->destination);
    return (*ProcXFixesVector[stuff->xfixesReqType]) (client);
}
