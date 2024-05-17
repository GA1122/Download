PassRefPtr<Element> HTMLConstructionSite::createElement(AtomicHTMLToken& token, const AtomicString& namespaceURI)
{
    QualifiedName tagName(nullAtom, token.name(), namespaceURI);
    RefPtr<Element> element = m_document->createElement(tagName, true);
    element->setAttributeMap(token.takeAtributes(), m_fragmentScriptingPermission);
    return element.release();
}
