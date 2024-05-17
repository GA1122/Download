PassRefPtr<Attr> Document::createAttribute(const String& name, ExceptionState& es)
{
    return createAttributeNS(String(), name, es, true);
}
