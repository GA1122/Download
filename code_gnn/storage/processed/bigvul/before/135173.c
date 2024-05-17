AtomicString Document::convertLocalName(const AtomicString& name)
{
    return isHTMLDocument() ? name.lower() : name;
}
