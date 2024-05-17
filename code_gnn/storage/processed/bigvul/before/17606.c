SProcRenderAddGlyphs(ClientPtr client)
{
    register int i;
    CARD32 *gids;
    void *end;
    xGlyphInfo *gi;

    REQUEST(xRenderAddGlyphsReq);
    REQUEST_AT_LEAST_SIZE(xRenderAddGlyphsReq);
    swaps(&stuff->length);
    swapl(&stuff->glyphset);
    swapl(&stuff->nglyphs);
    if (stuff->nglyphs & 0xe0000000)
        return BadLength;
    end = (CARD8 *) stuff + (client->req_len << 2);
    gids = (CARD32 *) (stuff + 1);
    gi = (xGlyphInfo *) (gids + stuff->nglyphs);
    if ((char *) end - (char *) (gids + stuff->nglyphs) < 0)
        return BadLength;
    if ((char *) end - (char *) (gi + stuff->nglyphs) < 0)
        return BadLength;
    for (i = 0; i < stuff->nglyphs; i++) {
        swapl(&gids[i]);
        swaps(&gi[i].width);
        swaps(&gi[i].height);
        swaps(&gi[i].x);
        swaps(&gi[i].y);
        swaps(&gi[i].xOff);
        swaps(&gi[i].yOff);
    }
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
