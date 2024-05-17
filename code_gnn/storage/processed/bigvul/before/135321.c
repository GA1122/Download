bool Document::parseQualifiedName(const AtomicString& qualifiedName, AtomicString& prefix, AtomicString& localName, ExceptionState& exceptionState)
{
    unsigned length = qualifiedName.length();

    if (!length) {
        exceptionState.throwDOMException(InvalidCharacterError, "The qualified name provided is empty.");
        return false;
    }

    ParseQualifiedNameResult returnValue;
    if (qualifiedName.is8Bit())
        returnValue = parseQualifiedNameInternal(qualifiedName, qualifiedName.characters8(), length, prefix, localName);
    else
        returnValue = parseQualifiedNameInternal(qualifiedName, qualifiedName.characters16(), length, prefix, localName);
    if (returnValue.status == QNValid)
        return true;

    StringBuilder message;
    message.appendLiteral("The qualified name provided ('");
    message.append(qualifiedName);
    message.appendLiteral("') ");

    if (returnValue.status == QNMultipleColons) {
        message.appendLiteral("contains multiple colons.");
    } else if (returnValue.status == QNInvalidStartChar) {
        message.appendLiteral("contains the invalid name-start character '");
        message.append(returnValue.character);
        message.appendLiteral("'.");
    } else if (returnValue.status == QNInvalidChar) {
        message.appendLiteral("contains the invalid character '");
        message.append(returnValue.character);
        message.appendLiteral("'.");
    } else if (returnValue.status == QNEmptyPrefix) {
        message.appendLiteral("has an empty namespace prefix.");
    } else {
        ASSERT(returnValue.status == QNEmptyLocalName);
        message.appendLiteral("has an empty local name.");
    }

    if (returnValue.status == QNInvalidStartChar || returnValue.status == QNInvalidChar)
        exceptionState.throwDOMException(InvalidCharacterError, message.toString());
    else
        exceptionState.throwDOMException(NamespaceError, message.toString());
    return false;
}
