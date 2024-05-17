int RenderBlock::columnGap() const
{
    if (style()->hasNormalColumnGap())
        return style()->fontDescription().computedPixelSize();  
    return static_cast<int>(style()->columnGap());
}
