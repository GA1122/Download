bool Document::isFrameSet() const
{
    if (!isHTMLDocument())
        return false;
    HTMLElement* bodyElement = body();
    return bodyElement && bodyElement->hasTagName(framesetTag);
}
