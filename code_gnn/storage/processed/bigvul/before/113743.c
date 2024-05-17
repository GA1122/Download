    static bool relAttributeIsStyleSheet(const String& attributeValue)
    {
        LinkRelAttribute rel(attributeValue);
        return rel.m_isStyleSheet && !rel.m_isAlternate && rel.m_iconType == InvalidIcon && !rel.m_isDNSPrefetch;
    }
