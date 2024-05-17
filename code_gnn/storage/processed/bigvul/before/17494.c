SProcXvQueryPortAttributes(ClientPtr client)
{
    REQUEST(xvQueryPortAttributesReq);
    REQUEST_SIZE_MATCH(xvQueryPortAttributesReq);
    swaps(&stuff->length);
    swapl(&stuff->port);
    return XvProcVector[xv_QueryPortAttributes] (client);
}
