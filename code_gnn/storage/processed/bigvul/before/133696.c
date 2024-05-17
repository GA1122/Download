bool XSSAuditor::eraseAttributeIfInjected(const FilterTokenRequest& request, const QualifiedName& attributeName, const String& replacementValue, AttributeKind treatment)
{
    size_t indexOfAttribute = 0;
    if (findAttributeWithName(request.token, attributeName, indexOfAttribute)) {
        const HTMLToken::Attribute& attribute = request.token.attributes().at(indexOfAttribute);
        if (isContainedInRequest(decodedSnippetForAttribute(request, attribute, treatment))) {
            if (threadSafeMatch(attributeName, srcAttr) && isLikelySafeResource(String(attribute.value)))
                return false;
            if (threadSafeMatch(attributeName, http_equivAttr) && !isDangerousHTTPEquiv(String(attribute.value)))
                return false;
            request.token.eraseValueOfAttribute(indexOfAttribute);
            if (!replacementValue.isEmpty())
                request.token.appendToAttributeValue(indexOfAttribute, replacementValue);
            return true;
        }
    }
    return false;
}
