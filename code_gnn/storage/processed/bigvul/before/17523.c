dmxBECreateGlyphSet(int idx, GlyphSetPtr glyphSet)
{
    XRenderPictFormat *pFormat;
    DMXScreenInfo *dmxScreen = &dmxScreens[idx];
    dmxGlyphPrivPtr glyphPriv = DMX_GET_GLYPH_PRIV(glyphSet);
    PictFormatPtr pFmt = glyphSet->format;
    int (*oldErrorHandler) (Display *, XErrorEvent *);

    pFormat = dmxFindFormat(dmxScreen, pFmt);
    if (!pFormat) {
        return BadMatch;
    }

    dmxGlyphLastError = 0;
    oldErrorHandler = XSetErrorHandler(dmxGlyphErrorHandler);

     
    glyphPriv->glyphSets[idx]
        = XRenderCreateGlyphSet(dmxScreen->beDisplay, pFormat);

    XSetErrorHandler(oldErrorHandler);

    if (dmxGlyphLastError) {
        return dmxGlyphLastError;
    }

    return Success;
}
