void HTMLConstructionSite::insertHTMLBodyElement(AtomicHTMLToken& token)
{
    ASSERT(!shouldFosterParent());
    m_openElements.pushHTMLBodyElement(attachToCurrent(createHTMLElement(token)));
}
