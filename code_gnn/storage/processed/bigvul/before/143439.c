    void processImgAttribute(const NameType& attributeName, const String& attributeValue)
    {
        if (match(attributeName, srcAttr) && m_imgSrcUrl.isNull()) {
            m_imgSrcUrl = attributeValue;
            setUrlToLoad(bestFitSourceForImageAttributes(m_mediaValues->devicePixelRatio(), m_sourceSize, attributeValue, m_srcsetImageCandidate), AllowURLReplacement);
        } else if (match(attributeName, crossoriginAttr)) {
            setCrossOrigin(attributeValue);
        } else if (match(attributeName, srcsetAttr) && m_srcsetImageCandidate.isEmpty()) {
            m_srcsetAttributeValue = attributeValue;
            m_srcsetImageCandidate = bestFitSourceForSrcsetAttribute(m_mediaValues->devicePixelRatio(), m_sourceSize, attributeValue);
            setUrlToLoad(bestFitSourceForImageAttributes(m_mediaValues->devicePixelRatio(), m_sourceSize, m_imgSrcUrl, m_srcsetImageCandidate), AllowURLReplacement);
        } else if (match(attributeName, sizesAttr) && !m_sourceSizeSet) {
            m_sourceSize = SizesAttributeParser(m_mediaValues, attributeValue).length();
            m_sourceSizeSet = true;
            if (!m_srcsetImageCandidate.isEmpty()) {
                m_srcsetImageCandidate = bestFitSourceForSrcsetAttribute(m_mediaValues->devicePixelRatio(), m_sourceSize, m_srcsetAttributeValue);
                setUrlToLoad(bestFitSourceForImageAttributes(m_mediaValues->devicePixelRatio(), m_sourceSize, m_imgSrcUrl, m_srcsetImageCandidate), AllowURLReplacement);
            }
        } else if (!m_referrerPolicySet && match(attributeName, referrerpolicyAttr) && !attributeValue.isNull()) {
            m_referrerPolicySet = true;
            SecurityPolicy::referrerPolicyFromString(attributeValue, &m_referrerPolicy);
        }
    }
