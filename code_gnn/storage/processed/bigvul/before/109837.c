bool Document::shouldDisplaySeamlesslyWithParent() const
{
    if (!RuntimeEnabledFeatures::seamlessIFramesEnabled())
        return false;
    HTMLFrameOwnerElement* ownerElement = this->ownerElement();
    if (!ownerElement)
        return false;
    return m_mayDisplaySeamlesslyWithParent && ownerElement->hasTagName(iframeTag) && ownerElement->fastHasAttribute(seamlessAttr);
}
