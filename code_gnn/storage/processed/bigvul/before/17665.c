SProcXFixesHideCursor(ClientPtr client)
{
    REQUEST(xXFixesHideCursorReq);

    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xXFixesHideCursorReq);
    swapl(&stuff->window);
    return (*ProcXFixesVector[stuff->xfixesReqType]) (client);
}
