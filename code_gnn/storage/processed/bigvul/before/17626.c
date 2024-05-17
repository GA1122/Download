SProcRenderQueryPictFormats(ClientPtr client)
{
    REQUEST(xRenderQueryPictFormatsReq);
    REQUEST_SIZE_MATCH(xRenderQueryPictFormatsReq);
    swaps(&stuff->length);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
