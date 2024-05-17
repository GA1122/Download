    explicit PreloadTask(const HTMLToken& token)
        : m_tagName(token.name().data(), token.name().size())
        , m_linkIsStyleSheet(false)
        , m_linkMediaAttributeIsScreen(true)
        , m_inputIsImage(false)
    {
        processAttributes(token.attributes());
    }
