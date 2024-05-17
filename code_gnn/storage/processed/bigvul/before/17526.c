dmxInitRender(void)
{
    int i;

    for (i = 0; i < RenderNumberRequests; i++)
        dmxSaveRenderVector[i] = ProcRenderVector[i];

    ProcRenderVector[X_RenderCreateGlyphSet]
        = dmxProcRenderCreateGlyphSet;
    ProcRenderVector[X_RenderFreeGlyphSet]
        = dmxProcRenderFreeGlyphSet;
    ProcRenderVector[X_RenderAddGlyphs]
        = dmxProcRenderAddGlyphs;
    ProcRenderVector[X_RenderFreeGlyphs]
        = dmxProcRenderFreeGlyphs;
    ProcRenderVector[X_RenderCompositeGlyphs8]
        = dmxProcRenderCompositeGlyphs;
    ProcRenderVector[X_RenderCompositeGlyphs16]
        = dmxProcRenderCompositeGlyphs;
    ProcRenderVector[X_RenderCompositeGlyphs32]
        = dmxProcRenderCompositeGlyphs;
    ProcRenderVector[X_RenderSetPictureTransform]
        = dmxProcRenderSetPictureTransform;
    ProcRenderVector[X_RenderSetPictureFilter]
        = dmxProcRenderSetPictureFilter;
}
