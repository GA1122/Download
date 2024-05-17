SProcRenderQueryPictIndexValues(ClientPtr client)
{
    REQUEST(xRenderQueryPictIndexValuesReq);
    REQUEST_AT_LEAST_SIZE(xRenderQueryPictIndexValuesReq);
    swaps(&stuff->length);
    swapl(&stuff->format);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
