SProcRenderFreeGlyphs(ClientPtr client)
{
    REQUEST(xRenderFreeGlyphsReq);
    REQUEST_AT_LEAST_SIZE(xRenderFreeGlyphsReq);
    swaps(&stuff->length);
    swapl(&stuff->glyphset);
    SwapRestL(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
