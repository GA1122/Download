bool XSSAuditor::eraseDangerousAttributesIfInjected(const FilterTokenRequest& request)
{
    DEFINE_STATIC_LOCAL(String, safeJavaScriptURL, ("javascript:void(0)"));

    bool didBlockScript = false;
    for (size_t i = 0; i < request.token.attributes().size(); ++i) {
        const HTMLToken::Attribute& attribute = request.token.attributes().at(i);
        bool isInlineEventHandler = isNameOfInlineEventHandler(attribute.name);
        String strippedValue = stripLeadingAndTrailingHTMLSpaces(String(attribute.value));
        bool valueContainsJavaScriptURL = (!isInlineEventHandler && protocolIsJavaScript(strippedValue)) || (isSemicolonSeparatedAttribute(attribute) && semicolonSeparatedValueContainsJavaScriptURL(strippedValue));
        if (!isInlineEventHandler && !valueContainsJavaScriptURL)
            continue;
        if (!isContainedInRequest(decodedSnippetForAttribute(request, attribute, ScriptLikeAttribute)))
            continue;
        request.token.eraseValueOfAttribute(i);
        if (valueContainsJavaScriptURL)
            request.token.appendToAttributeValue(i, safeJavaScriptURL);
        didBlockScript = true;
    }
    return didBlockScript;
}
