static bool parseQualifiedNameInternal(const String& qualifiedName, const CharType* characters, unsigned length, String& prefix, String& localName, ExceptionState& es)
{
    bool nameStart = true;
    bool sawColon = false;
    int colonPos = 0;

    for (unsigned i = 0; i < length;) {
        UChar32 c;
        U16_NEXT(characters, i, length, c)
        if (c == ':') {
            if (sawColon) {
                es.throwUninformativeAndGenericDOMException(NamespaceError);
                return false;  
            }
            nameStart = true;
            sawColon = true;
            colonPos = i - 1;
        } else if (nameStart) {
            if (!isValidNameStart(c)) {
                es.throwUninformativeAndGenericDOMException(InvalidCharacterError);
                return false;
            }
            nameStart = false;
        } else {
            if (!isValidNamePart(c)) {
                es.throwUninformativeAndGenericDOMException(InvalidCharacterError);
                return false;
            }
        }
    }

    if (!sawColon) {
        prefix = String();
        localName = qualifiedName;
    } else {
        prefix = qualifiedName.substring(0, colonPos);
        if (prefix.isEmpty()) {
            es.throwUninformativeAndGenericDOMException(NamespaceError);
            return false;
        }
        localName = qualifiedName.substring(colonPos + 1);
    }

    if (localName.isEmpty()) {
        es.throwUninformativeAndGenericDOMException(NamespaceError);
        return false;
    }

    return true;
}
