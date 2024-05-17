    void processSourceAttribute(const NameType& attributeName, const String& attributeValue)
    {
        if (match(attributeName, srcsetAttr) && m_srcsetImageCandidate.isEmpty()) {
            m_srcsetAttributeValue = attributeValue;
            m_srcsetImageCandidate = bestFitSourceForSrcsetAttribute(m_mediaValues->devicePixelRatio(), m_sourceSize, attributeValue);
        } else if (match(attributeName, sizesAttr) && !m_sourceSizeSet) {
            m_sourceSize = SizesAttributeParser(m_mediaValues, attributeValue).length();
            m_sourceSizeSet = true;
            if (!m_srcsetImageCandidate.isEmpty()) {
                m_srcsetImageCandidate = bestFitSourceForSrcsetAttribute(m_mediaValues->devicePixelRatio(), m_sourceSize, m_srcsetAttributeValue);
            }
        } else if (match(attributeName, mediaAttr)) {
            m_matchedMediaAttribute = mediaAttributeMatches(*m_mediaValues, attributeValue);
        }
    }
