HTMLIFrameElement* Document::seamlessParentIFrame() const
{
    if (!shouldDisplaySeamlesslyWithParent())
        return 0;

    HTMLFrameOwnerElement* ownerElement = this->ownerElement();
    ASSERT(ownerElement->hasTagName(iframeTag));
    return static_cast<HTMLIFrameElement*>(ownerElement);
}
