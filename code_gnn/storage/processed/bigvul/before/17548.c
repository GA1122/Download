SProcPseudoramiXQueryScreens(ClientPtr client)
{
    REQUEST(xXineramaQueryScreensReq);

    TRACE;

    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xXineramaQueryScreensReq);
    return ProcPseudoramiXQueryScreens(client);
}
