bool XSSAuditor::filterObjectToken(const FilterTokenRequest& request)
{
    ASSERT(request.token.type() == HTMLToken::StartTag);
    ASSERT(hasName(request.token, objectTag));

    bool didBlockScript = false;
    if (isContainedInRequest(decodedSnippetForName(request))) {
        didBlockScript |= eraseAttributeIfInjected(request, dataAttr, blankURL().string(), SrcLikeAttribute);
        didBlockScript |= eraseAttributeIfInjected(request, typeAttr);
        didBlockScript |= eraseAttributeIfInjected(request, classidAttr);
    }
    return didBlockScript;
}
