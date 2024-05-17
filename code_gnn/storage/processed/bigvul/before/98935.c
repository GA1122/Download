void HTMLConstructionSite::insertScriptElement(AtomicHTMLToken& token)
{
    RefPtr<HTMLScriptElement> element = HTMLScriptElement::create(scriptTag, m_document, true);
    element->setAttributeMap(token.takeAtributes(), m_fragmentScriptingPermission);
    m_openElements.push(attachToCurrent(element.release()));
}
