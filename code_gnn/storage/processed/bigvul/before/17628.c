SProcRenderQueryVersion(ClientPtr client)
{
    REQUEST(xRenderQueryVersionReq);
    REQUEST_SIZE_MATCH(xRenderQueryVersionReq);
    swaps(&stuff->length);
    swapl(&stuff->majorVersion);
    swapl(&stuff->minorVersion);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
