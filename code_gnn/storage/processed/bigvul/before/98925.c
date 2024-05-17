void HTMLConstructionSite::insertForeignElement(AtomicHTMLToken& token, const AtomicString& namespaceURI)
{
    ASSERT(token.type() == HTMLToken::StartTag);
    notImplemented();  

    RefPtr<Element> element = attachToCurrent(createElement(token, namespaceURI));
    if (!token.selfClosing())
        m_openElements.push(element);
}
