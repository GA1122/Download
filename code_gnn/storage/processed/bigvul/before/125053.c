static ItemPosition resolveAlignment(const RenderStyle* parentStyle, const RenderStyle* childStyle)
{
    ItemPosition align = childStyle->alignSelf();
    if (align == ItemPositionAuto)
        align = parentStyle->alignItems();
    return align;
}
