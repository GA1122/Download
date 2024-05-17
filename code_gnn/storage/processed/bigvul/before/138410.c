PassRefPtrWillBeRawPtr<Attr> Document::createAttribute(const AtomicString& name, ExceptionState& exceptionState)
{
    return createAttributeNS(nullAtom, name, exceptionState, true);
}
