    bool shouldPreload() const
    {
        if (m_urlToLoad.isEmpty())
            return false;
        if (match(m_tagImpl, linkTag) && !m_linkIsStyleSheet && !m_linkIsImport && !m_linkIsPreload)
            return false;
        if (match(m_tagImpl, linkTag) && m_linkIsStyleSheet && !m_linkTypeIsMissingOrSupportedStyleSheet)
            return false;
        if (match(m_tagImpl, inputTag) && !m_inputIsImage)
            return false;
        return true;
    }
