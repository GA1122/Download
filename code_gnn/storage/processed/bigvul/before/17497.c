SProcXvSetPortAttribute(ClientPtr client)
{
    REQUEST(xvSetPortAttributeReq);
    REQUEST_SIZE_MATCH(xvSetPortAttributeReq);
    swaps(&stuff->length);
    swapl(&stuff->port);
    swapl(&stuff->attribute);
    swapl(&stuff->value);
    return XvProcVector[xv_SetPortAttribute] (client);
}
