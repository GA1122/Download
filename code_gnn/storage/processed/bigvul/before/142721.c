bool FrameLoader::shouldTreatURLAsSrcdocDocument(const KURL& url) const
{
    if (!url.isAboutSrcdocURL())
        return false;
    HTMLFrameOwnerElement* ownerElement = m_frame->deprecatedLocalOwner();
    if (!isHTMLIFrameElement(ownerElement))
        return false;
    return ownerElement->fastHasAttribute(srcdocAttr);
}
