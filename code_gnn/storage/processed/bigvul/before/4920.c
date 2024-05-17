SProcGetProperty(ClientPtr client)
{
    REQUEST(xGetPropertyReq);
    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xGetPropertyReq);
    swapl(&stuff->window);
    swapl(&stuff->property);
    swapl(&stuff->type);
    swapl(&stuff->longOffset);
    swapl(&stuff->longLength);
    return ((*ProcVector[X_GetProperty]) (client));
}
