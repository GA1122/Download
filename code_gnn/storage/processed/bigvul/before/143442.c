    void processScriptAttribute(const NameType& attributeName, const String& attributeValue)
    {
        if (match(attributeName, srcAttr))
            setUrlToLoad(attributeValue, DisallowURLReplacement);
        else if (match(attributeName, crossoriginAttr))
            setCrossOrigin(attributeValue);
        else if (match(attributeName, asyncAttr))
            setDefer(FetchRequest::LazyLoad);
        else if (match(attributeName, deferAttr))
            setDefer(FetchRequest::LazyLoad);
        else if (match(attributeName, integrityAttr))
            SubresourceIntegrity::parseIntegrityAttribute(attributeValue, m_integrityMetadata);
    }
