bool XSSAuditor::filterParamToken(const FilterTokenRequest& request)
{
    ASSERT(request.token.type() == HTMLToken::StartTag);
    ASSERT(hasName(request.token, paramTag));

    size_t indexOfNameAttribute;
    if (!findAttributeWithName(request.token, nameAttr, indexOfNameAttribute))
        return false;

    const HTMLToken::Attribute& nameAttribute = request.token.attributes().at(indexOfNameAttribute);
    if (!HTMLParamElement::isURLParameter(String(nameAttribute.value)))
        return false;

    return eraseAttributeIfInjected(request, valueAttr, blankURL().string(), SrcLikeAttribute);
}
