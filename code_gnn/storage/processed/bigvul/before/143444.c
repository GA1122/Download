    void processVideoAttribute(const NameType& attributeName, const String& attributeValue)
    {
        if (match(attributeName, posterAttr))
            setUrlToLoad(attributeValue, DisallowURLReplacement);
    }
