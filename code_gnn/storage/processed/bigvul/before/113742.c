    void processAttributes(const HTMLToken::AttributeList& attributes)
    {
        if (m_tagName != imgTag
            && m_tagName != inputTag
            && m_tagName != linkTag
            && m_tagName != scriptTag
            && m_tagName != baseTag)
            return;

        for (HTMLToken::AttributeList::const_iterator iter = attributes.begin();
             iter != attributes.end(); ++iter) {
            AtomicString attributeName(iter->m_name.data(), iter->m_name.size());
            String attributeValue = StringImpl::create8BitIfPossible(iter->m_value.data(), iter->m_value.size());

            if (attributeName == charsetAttr)
                m_charset = attributeValue;

            if (m_tagName == scriptTag || m_tagName == imgTag) {
                if (attributeName == srcAttr)
                    setUrlToLoad(attributeValue);
                else if (attributeName == crossoriginAttr && !attributeValue.isNull())
                    m_crossOriginMode = stripLeadingAndTrailingHTMLSpaces(attributeValue);
            } else if (m_tagName == linkTag) {
                if (attributeName == hrefAttr)
                    setUrlToLoad(attributeValue);
                else if (attributeName == relAttr)
                    m_linkIsStyleSheet = relAttributeIsStyleSheet(attributeValue);
                else if (attributeName == mediaAttr)
                    m_linkMediaAttributeIsScreen = linkMediaAttributeIsScreen(attributeValue);
            } else if (m_tagName == inputTag) {
                if (attributeName == srcAttr)
                    setUrlToLoad(attributeValue);
                else if (attributeName == typeAttr)
                    m_inputIsImage = equalIgnoringCase(attributeValue, InputTypeNames::image());
            } else if (m_tagName == baseTag) {
                if (attributeName == hrefAttr)
                    m_baseElementHref = stripLeadingAndTrailingHTMLSpaces(attributeValue);
            }
        }
    }
