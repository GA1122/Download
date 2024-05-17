SProcXvQueryImageAttributes(ClientPtr client)
{
    REQUEST(xvQueryImageAttributesReq);
    REQUEST_SIZE_MATCH(xvQueryImageAttributesReq);
    swaps(&stuff->length);
    swapl(&stuff->port);
    swapl(&stuff->id);
    swaps(&stuff->width);
    swaps(&stuff->height);
    return XvProcVector[xv_QueryImageAttributes] (client);
}
