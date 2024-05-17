dmxBEFreeGlyphSet(ScreenPtr pScreen, GlyphSetPtr glyphSet)
{
    dmxGlyphPrivPtr glyphPriv = DMX_GET_GLYPH_PRIV(glyphSet);
    int idx = pScreen->myNum;
    DMXScreenInfo *dmxScreen = &dmxScreens[idx];

    if (glyphPriv->glyphSets[idx]) {
        XRenderFreeGlyphSet(dmxScreen->beDisplay, glyphPriv->glyphSets[idx]);
        glyphPriv->glyphSets[idx] = (GlyphSet) 0;
        return TRUE;
    }

    return FALSE;
}
