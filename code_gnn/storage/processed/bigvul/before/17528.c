dmxProcRenderAddGlyphs(ClientPtr client)
{
    int ret;

    REQUEST(xRenderAddGlyphsReq);

    ret = dmxSaveRenderVector[stuff->renderReqType] (client);

    if (ret == Success) {
        GlyphSetPtr glyphSet;
        dmxGlyphPrivPtr glyphPriv;
        int i;
        int nglyphs;
        CARD32 *gids;
        Glyph *gidsCopy;
        xGlyphInfo *gi;
        CARD8 *bits;
        int nbytes;

        dixLookupResourceByType((void **) &glyphSet,
                                stuff->glyphset, GlyphSetType,
                                client, DixReadAccess);
        glyphPriv = DMX_GET_GLYPH_PRIV(glyphSet);

        nglyphs = stuff->nglyphs;
        gids = (CARD32 *) (stuff + 1);
        gi = (xGlyphInfo *) (gids + nglyphs);
        bits = (CARD8 *) (gi + nglyphs);
        nbytes = ((stuff->length << 2) -
                  sizeof(xRenderAddGlyphsReq) -
                  (sizeof(CARD32) + sizeof(xGlyphInfo)) * nglyphs);

        gidsCopy = xallocarray(nglyphs, sizeof(*gidsCopy));
        for (i = 0; i < nglyphs; i++)
            gidsCopy[i] = gids[i];

         
        for (i = 0; i < dmxNumScreens; i++) {
            DMXScreenInfo *dmxScreen = &dmxScreens[i];

            if (dmxScreen->beDisplay) {
                XRenderAddGlyphs(dmxScreen->beDisplay,
                                 glyphPriv->glyphSets[i],
                                 gidsCopy,
                                 (XGlyphInfo *) gi,
                                 nglyphs, (char *) bits, nbytes);
                dmxSync(dmxScreen, FALSE);
            }
        }
        free(gidsCopy);
    }

    return ret;
}
