    const String& charset() const
    {
        if (match(m_tagImpl, imgTag) || match(m_tagImpl, videoTag))
            return emptyString();
        return m_charset;
    }
