SProcPseudoramiXQueryVersion(ClientPtr client)
{
    REQUEST(xPanoramiXQueryVersionReq);

    TRACE;

    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xPanoramiXQueryVersionReq);
    return ProcPseudoramiXQueryVersion(client);
}
