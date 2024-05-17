    void processAttributes(const Vector<CompactHTMLToken::Attribute>& attributes)
    {
        if (!m_tagImpl)
            return;
        for (const CompactHTMLToken::Attribute& htmlTokenAttribute : attributes)
            processAttribute(htmlTokenAttribute.name(), htmlTokenAttribute.value());
    }
