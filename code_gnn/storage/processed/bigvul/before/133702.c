bool XSSAuditor::filterEmbedToken(const FilterTokenRequest& request)
{
    ASSERT(request.token.type() == HTMLToken::StartTag);
    ASSERT(hasName(request.token, embedTag));

    bool didBlockScript = false;
    if (isContainedInRequest(decodedSnippetForName(request))) {
        didBlockScript |= eraseAttributeIfInjected(request, codeAttr, String(), SrcLikeAttribute);
        didBlockScript |= eraseAttributeIfInjected(request, srcAttr, blankURL().string(), SrcLikeAttribute);
        didBlockScript |= eraseAttributeIfInjected(request, typeAttr);
    }
    return didBlockScript;
}
