    bool shouldPreconnect() const
    {
        return match(m_tagImpl, linkTag) && m_linkIsPreconnect && !m_urlToLoad.isEmpty();
    }
