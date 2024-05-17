PassRefPtr<RenderStyle> Document::styleForPage(int pageIndex)
{
    return styleResolver()->styleForPage(pageIndex);
}
