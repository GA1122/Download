SProcRenderQueryPictFormats (ClientPtr client)
{
    register int n;
    REQUEST(xRenderQueryPictFormatsReq);
    swaps(&stuff->length, n);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
