dmxProcRenderFreeGlyphSet(ClientPtr client)
{
    GlyphSetPtr glyphSet;

    REQUEST(xRenderFreeGlyphSetReq);

    REQUEST_SIZE_MATCH(xRenderFreeGlyphSetReq);
    dixLookupResourceByType((void **) &glyphSet,
                            stuff->glyphset, GlyphSetType,
                            client, DixDestroyAccess);

    if (glyphSet && glyphSet->refcnt == 1) {
        dmxGlyphPrivPtr glyphPriv = DMX_GET_GLYPH_PRIV(glyphSet);
        int i;

        for (i = 0; i < dmxNumScreens; i++) {
            DMXScreenInfo *dmxScreen = &dmxScreens[i];

            if (dmxScreen->beDisplay) {
                if (dmxBEFreeGlyphSet(screenInfo.screens[i], glyphSet))
                    dmxSync(dmxScreen, FALSE);
            }
        }

        MAXSCREENSFREE(glyphPriv->glyphSets);
        free(glyphPriv);
        DMX_SET_GLYPH_PRIV(glyphSet, NULL);
    }

    return dmxSaveRenderVector[stuff->renderReqType] (client);
}
