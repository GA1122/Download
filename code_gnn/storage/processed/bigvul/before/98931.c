void HTMLConstructionSite::insertHTMLHeadElement(AtomicHTMLToken& token)
{
    ASSERT(!shouldFosterParent());
    m_head = attachToCurrent(createHTMLElement(token));
    m_openElements.pushHTMLHeadElement(m_head);
}
