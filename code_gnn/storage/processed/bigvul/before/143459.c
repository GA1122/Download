void TokenPreloadScanner::updatePredictedBaseURL(const Token& token)
{
    ASSERT(m_predictedBaseElementURL.isEmpty());
    if (const typename Token::Attribute* hrefAttribute = token.getAttributeItem(hrefAttr)) {
        KURL url(m_documentURL, stripLeadingAndTrailingHTMLSpaces(hrefAttribute->value8BitIfNecessary()));
        m_predictedBaseElementURL = url.isValid() ? url.copy() : KURL();
    }
}
