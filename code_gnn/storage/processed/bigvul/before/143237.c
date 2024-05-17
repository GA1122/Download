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
    message.append("The qualified name provided ('");
    message.append(qualifiedName);
    message.append("') ");

    if (returnValue.status == QNMultipleColons) {
        message.append("contains multiple colons.");
    } else if (returnValue.status == QNInvalidStartChar) {
        message.append("contains the invalid name-start character '");
        message.append(returnValue.character);
        message.append("'.");
    } else if (returnValue.status == QNInvalidChar) {
        message.append("contains the invalid character '");
        message.append(returnValue.character);
        message.append("'.");
    } else if (returnValue.status == QNEmptyPrefix) {
        message.append("has an empty namespace prefix.");
    } else {
        DCHECK_EQ(returnValue.status, QNEmptyLocalName);
        message.append("has an empty local name.");
    }

    if (returnValue.status == QNInvalidStartChar || returnValue.status == QNInvalidChar)
        exceptionState.throwDOMException(InvalidCharacterError, message.toString());
    else
        exceptionState.throwDOMException(NamespaceError, message.toString());
    return false;
}
