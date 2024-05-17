SProcRenderFreeGlyphSet(ClientPtr client)
{
    REQUEST(xRenderFreeGlyphSetReq);
    REQUEST_SIZE_MATCH(xRenderFreeGlyphSetReq);
    swaps(&stuff->length);
    swapl(&stuff->glyphset);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
