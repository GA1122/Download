void HTMLConstructionSite::insertHTMLBodyStartTagInBody(AtomicHTMLToken& token)
{
    mergeAttributesFromTokenIntoElement(token, m_openElements.bodyElement());
}
