dmxProcRenderFreeGlyphs(ClientPtr client)
{
    GlyphSetPtr glyphSet;

    REQUEST(xRenderFreeGlyphsReq);

    REQUEST_AT_LEAST_SIZE(xRenderFreeGlyphsReq);
    dixLookupResourceByType((void **) &glyphSet,
                            stuff->glyphset, GlyphSetType,
                            client, DixWriteAccess);

    if (glyphSet) {
        dmxGlyphPrivPtr glyphPriv = DMX_GET_GLYPH_PRIV(glyphSet);
        int i;
        int nglyphs;
        Glyph *gids;

        nglyphs = ((client->req_len << 2) - sizeof(xRenderFreeGlyphsReq)) >> 2;
        if (nglyphs) {
            gids = xallocarray(nglyphs, sizeof(*gids));
            for (i = 0; i < nglyphs; i++)
                gids[i] = ((CARD32 *) (stuff + 1))[i];

            for (i = 0; i < dmxNumScreens; i++) {
                DMXScreenInfo *dmxScreen = &dmxScreens[i];

                if (dmxScreen->beDisplay) {
                    XRenderFreeGlyphs(dmxScreen->beDisplay,
                                      glyphPriv->glyphSets[i], gids, nglyphs);
                    dmxSync(dmxScreen, FALSE);
                }
            }
            free(gids);
        }
    }

    return dmxSaveRenderVector[stuff->renderReqType] (client);
}
