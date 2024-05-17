SProcRenderQueryFilters(ClientPtr client)
{
    REQUEST(xRenderQueryFiltersReq);
    REQUEST_SIZE_MATCH(xRenderQueryFiltersReq);

    swaps(&stuff->length);
    swapl(&stuff->drawable);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
