SProcScreenSaverSetAttributes(ClientPtr client)
{
    REQUEST(xScreenSaverSetAttributesReq);
    swaps(&stuff->length);
    REQUEST_AT_LEAST_SIZE(xScreenSaverSetAttributesReq);
    swapl(&stuff->drawable);
    swaps(&stuff->x);
    swaps(&stuff->y);
    swaps(&stuff->width);
    swaps(&stuff->height);
    swaps(&stuff->borderWidth);
    swapl(&stuff->visualID);
    swapl(&stuff->mask);
    SwapRestL(stuff);
    return ProcScreenSaverSetAttributes(client);
}
