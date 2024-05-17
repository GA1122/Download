    void setUrlToLoad(const String& attributeValue)
    {
        if (!m_urlToLoad.isEmpty())
            return;
        m_urlToLoad = stripLeadingAndTrailingHTMLSpaces(attributeValue);
    }
