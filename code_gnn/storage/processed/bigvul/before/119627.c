void RenderBlock::layoutLineGridBox()
{
    if (style()->lineGrid() == RenderStyle::initialLineGrid()) {
        setLineGridBox(0);
        return;
    }

    setLineGridBox(0);

    RootInlineBox* lineGridBox = new RootInlineBox(this);
    lineGridBox->setHasTextChildren();  
    lineGridBox->setConstructed();
    GlyphOverflowAndFallbackFontsMap textBoxDataMap;
    VerticalPositionCache verticalPositionCache;
    lineGridBox->alignBoxesInBlockDirection(logicalHeight(), textBoxDataMap, verticalPositionCache);

    setLineGridBox(lineGridBox);

}
