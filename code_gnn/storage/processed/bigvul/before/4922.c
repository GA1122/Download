SProcReparentWindow(ClientPtr client)
{
    REQUEST(xReparentWindowReq);
    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xReparentWindowReq);
    swapl(&stuff->window);
    swapl(&stuff->parent);
    swaps(&stuff->x);
    swaps(&stuff->y);
    return ((*ProcVector[X_ReparentWindow]) (client));
}
