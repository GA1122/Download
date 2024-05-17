PassRefPtrWillBeRawPtr<Attr> Document::createAttribute(const AtomicString& name, ExceptionState& exceptionState)
{
    if (isHTMLDocument() && name != name.lower())
        UseCounter::count(*this, UseCounter::HTMLDocumentCreateAttributeNameNotLowercase);
    return createAttributeNS(nullAtom, name, exceptionState, true);
}
