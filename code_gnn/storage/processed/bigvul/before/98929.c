void HTMLConstructionSite::insertHTMLElement(AtomicHTMLToken& token)
{
    m_openElements.push(attachToCurrent(createHTMLElement(token)));
}
