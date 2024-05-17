SProcXvGetPortAttribute(ClientPtr client)
{
    REQUEST(xvGetPortAttributeReq);
    REQUEST_SIZE_MATCH(xvGetPortAttributeReq);
    swaps(&stuff->length);
    swapl(&stuff->port);
    swapl(&stuff->attribute);
    return XvProcVector[xv_GetPortAttribute] (client);
}
