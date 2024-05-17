void HTMLConstructionSite::insertFormattingElement(AtomicHTMLToken& token)
{
    insertHTMLElement(token);
    m_activeFormattingElements.append(currentElement());
}
