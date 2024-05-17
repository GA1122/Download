    void processAttribute(const NameType& attributeName, const String& attributeValue)
    {
        if (match(attributeName, charsetAttr))
            m_charset = attributeValue;

        if (match(m_tagImpl, scriptTag))
            processScriptAttribute(attributeName, attributeValue);
        else if (match(m_tagImpl, imgTag))
            processImgAttribute(attributeName, attributeValue);
        else if (match(m_tagImpl, linkTag))
            processLinkAttribute(attributeName, attributeValue);
        else if (match(m_tagImpl, inputTag))
            processInputAttribute(attributeName, attributeValue);
        else if (match(m_tagImpl, sourceTag))
            processSourceAttribute(attributeName, attributeValue);
        else if (match(m_tagImpl, videoTag))
            processVideoAttribute(attributeName, attributeValue);
    }
