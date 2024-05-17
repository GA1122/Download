HTMLFrameOwnerElement* Frame::deprecatedLocalOwner() const
{
    return m_owner && m_owner->isLocal() ? toHTMLFrameOwnerElement(m_owner) : nullptr;
}
