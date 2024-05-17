Element* Document::viewportDefiningElement(const ComputedStyle* rootStyle) const
{
    Element* rootElement = documentElement();
    Element* bodyElement = body();
    if (!rootElement)
        return 0;
    if (!rootStyle) {
        rootStyle = rootElement->computedStyle();
        if (!rootStyle)
            return 0;
    }
    if (bodyElement && rootStyle->isOverflowVisible() && isHTMLHtmlElement(*rootElement))
        return bodyElement;
    return rootElement;
}
