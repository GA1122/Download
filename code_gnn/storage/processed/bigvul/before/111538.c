static Element* elementForTapHighlight(Element* elementUnderFatFinger)
{
    if (elementUnderFatFinger->renderer()) {
        Color tapHighlightColor = elementUnderFatFinger->renderStyle()->tapHighlightColor();
        if (tapHighlightColor != RenderTheme::defaultTheme()->platformTapHighlightColor())
            return elementUnderFatFinger;
    }

    bool isArea = elementUnderFatFinger->hasTagName(HTMLNames::areaTag);
    Node* linkNode = elementUnderFatFinger->enclosingLinkEventParentOrSelf();
    if (!linkNode || !linkNode->isHTMLElement() || (!linkNode->renderer() && !isArea))
        return 0;

    ASSERT(linkNode->isLink());

    Element* highlightCandidateElement = static_cast<Element*>(linkNode);

    if (!isArea)
        return highlightCandidateElement;

    HTMLAreaElement* area = static_cast<HTMLAreaElement*>(highlightCandidateElement);
    HTMLImageElement* image = area->imageElement();
    if (image && image->renderer())
        return image;

    return 0;
}
