bool Document::isPageBoxVisible(int pageIndex)
{
    RefPtr<RenderStyle> style = styleForPage(pageIndex);
    return style->visibility() != HIDDEN;  
}
