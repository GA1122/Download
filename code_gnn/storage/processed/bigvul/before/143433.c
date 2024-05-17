    bool isLinkRelPreload() const
    {
        return match(m_tagImpl, linkTag) && m_linkIsPreload && !m_urlToLoad.isEmpty();
    }
