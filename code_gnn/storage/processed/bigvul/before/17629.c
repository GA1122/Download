SProcRenderReferenceGlyphSet(ClientPtr client)
{
    REQUEST(xRenderReferenceGlyphSetReq);
    REQUEST_SIZE_MATCH(xRenderReferenceGlyphSetReq);
    swaps(&stuff->length);
    swapl(&stuff->gsid);
    swapl(&stuff->existing);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
