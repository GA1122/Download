void HTMLConstructionSite::insertHTMLHtmlElement(AtomicHTMLToken& token)
{
    ASSERT(!shouldFosterParent());
    m_openElements.pushHTMLHtmlElement(attachToCurrent(createHTMLElement(token)));
    dispatchDocumentElementAvailableIfNeeded();
}
