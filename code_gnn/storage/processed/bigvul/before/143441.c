    void processLinkAttribute(const NameType& attributeName, const String& attributeValue)
    {
        if (match(attributeName, hrefAttr)) {
            setUrlToLoad(attributeValue, DisallowURLReplacement);
        } else if (match(attributeName, relAttr)) {
            LinkRelAttribute rel(attributeValue);
            m_linkIsStyleSheet = rel.isStyleSheet() && !rel.isAlternate() && rel.getIconType() == InvalidIcon && !rel.isDNSPrefetch();
            m_linkIsPreconnect = rel.isPreconnect();
            m_linkIsPreload = rel.isLinkPreload();
            m_linkIsImport = rel.isImport();
        } else if (match(attributeName, mediaAttr)) {
            m_matchedMediaAttribute = mediaAttributeMatches(*m_mediaValues, attributeValue);
        } else if (match(attributeName, crossoriginAttr)) {
            setCrossOrigin(attributeValue);
        } else if (match(attributeName, asAttr)) {
            m_asAttributeValue = attributeValue;
        } else if (match(attributeName, typeAttr)) {
            m_linkTypeIsMissingOrSupportedStyleSheet = MIMETypeRegistry::isSupportedStyleSheetMIMEType(ContentType(attributeValue).type());
        }
    }
