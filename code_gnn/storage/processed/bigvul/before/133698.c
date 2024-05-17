bool XSSAuditor::filterAppletToken(const FilterTokenRequest& request)
{
    ASSERT(request.token.type() == HTMLToken::StartTag);
    ASSERT(hasName(request.token, appletTag));

    bool didBlockScript = false;
    if (isContainedInRequest(decodedSnippetForName(request))) {
        didBlockScript |= eraseAttributeIfInjected(request, codeAttr, String(), SrcLikeAttribute);
        didBlockScript |= eraseAttributeIfInjected(request, objectAttr);
    }
    return didBlockScript;
}
