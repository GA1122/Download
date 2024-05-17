bool XSSAuditor::filterInputToken(const FilterTokenRequest& request)
{
    ASSERT(request.token.type() == HTMLToken::StartTag);
    ASSERT(hasName(request.token, inputTag));

    return eraseAttributeIfInjected(request, formactionAttr, kURLWithUniqueOrigin, SrcLikeAttribute);
}
