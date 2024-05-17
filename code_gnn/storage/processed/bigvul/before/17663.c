SProcXFixesGetCursorImage(ClientPtr client)
{
    REQUEST(xXFixesGetCursorImageReq);
    swaps(&stuff->length);
    return (*ProcXFixesVector[stuff->xfixesReqType]) (client);
}
