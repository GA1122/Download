SProcRenderFreeGlyphs (ClientPtr client)
{
    register int n;
    REQUEST(xRenderFreeGlyphsReq);
    swaps(&stuff->length, n);
    swapl(&stuff->glyphset, n);
    SwapRestL(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
