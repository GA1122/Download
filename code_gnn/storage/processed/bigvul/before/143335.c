WebFrame* WebFrame::fromFrameOwnerElement(const WebElement& webElement)
{
    Element* element = webElement;

    if (!element->isFrameOwnerElement())
        return nullptr;
     return fromFrame(toHTMLFrameOwnerElement(element)->contentFrame());
 }
