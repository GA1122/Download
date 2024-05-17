bool XSSAuditor::filterMetaToken(const FilterTokenRequest& request)
{
    ASSERT(request.token.type() == HTMLToken::StartTag);
    ASSERT(hasName(request.token, metaTag));

    return eraseAttributeIfInjected(request, http_equivAttr);
}
