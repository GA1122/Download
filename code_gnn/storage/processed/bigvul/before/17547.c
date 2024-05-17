SProcPseudoramiXIsActive(ClientPtr client)
{
    REQUEST(xXineramaIsActiveReq);

    TRACE;

    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xXineramaIsActiveReq);
    return ProcPseudoramiXIsActive(client);
}
