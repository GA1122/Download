static ParseQualifiedNameResult parseQualifiedNameInternal(const AtomicString& qualifiedName, const CharType* characters, unsigned length, AtomicString& prefix, AtomicString& localName)
{
    bool nameStart = true;
    bool sawColon = false;
    int colonPos = 0;

    for (unsigned i = 0; i < length;) {
        UChar32 c;
        U16_NEXT(characters, i, length, c)
        if (c == ':') {
            if (sawColon)
                return ParseQualifiedNameResult(QNMultipleColons);
            nameStart = true;
            sawColon = true;
            colonPos = i - 1;
        } else if (nameStart) {
            if (!isValidNameStart(c))
                return ParseQualifiedNameResult(QNInvalidStartChar, c);
            nameStart = false;
        } else {
            if (!isValidNamePart(c))
                return ParseQualifiedNameResult(QNInvalidChar, c);
        }
    }

    if (!sawColon) {
        prefix = nullAtom;
        localName = qualifiedName;
    } else {
        prefix = AtomicString(characters, colonPos);
        if (prefix.isEmpty())
            return ParseQualifiedNameResult(QNEmptyPrefix);
        int prefixStart = colonPos + 1;
        localName = AtomicString(characters + prefixStart, length - prefixStart);
    }

    if (localName.isEmpty())
        return ParseQualifiedNameResult(QNEmptyLocalName);

    return ParseQualifiedNameResult(QNValid);
}
