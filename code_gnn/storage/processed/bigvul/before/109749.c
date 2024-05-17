bool Document::parseQualifiedName(const String& qualifiedName, String& prefix, String& localName, ExceptionState& es)
{
    unsigned length = qualifiedName.length();

    if (!length) {
        es.throwUninformativeAndGenericDOMException(InvalidCharacterError);
        return false;
    }

    if (qualifiedName.is8Bit())
        return parseQualifiedNameInternal(qualifiedName, qualifiedName.characters8(), length, prefix, localName, es);
    return parseQualifiedNameInternal(qualifiedName, qualifiedName.characters16(), length, prefix, localName, es);
}
