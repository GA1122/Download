bool XSSAuditor::filterButtonToken(const FilterTokenRequest& request)
{
    ASSERT(request.token.type() == HTMLToken::StartTag);
    ASSERT(hasName(request.token, buttonTag));

    return eraseAttributeIfInjected(request, formactionAttr, kURLWithUniqueOrigin, SrcLikeAttribute);
}
