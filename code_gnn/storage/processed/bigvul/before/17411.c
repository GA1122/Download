SProcScreenSaverQueryVersion(ClientPtr client)
{
    REQUEST(xScreenSaverQueryVersionReq);
    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xScreenSaverQueryVersionReq);
    return ProcScreenSaverQueryVersion(client);
}
