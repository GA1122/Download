SProcScreenSaverSelectInput(ClientPtr client)
{
    REQUEST(xScreenSaverSelectInputReq);
    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xScreenSaverSelectInputReq);
    swapl(&stuff->drawable);
    swapl(&stuff->eventMask);
    return ProcScreenSaverSelectInput(client);
}
