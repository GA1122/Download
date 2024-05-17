PassRefPtr<RenderStyle> Document::styleForPage(int pageIndex)
{
    RefPtr<RenderStyle> style = styleResolver()->styleForPage(pageIndex);
    return style.release();
}
