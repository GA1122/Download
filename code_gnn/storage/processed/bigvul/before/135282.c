bool Document::isFrameSet() const
{
    if (!isHTMLDocument())
        return false;
    return isHTMLFrameSetElement(body());
}
