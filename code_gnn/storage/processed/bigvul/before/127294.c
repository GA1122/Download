    PassRefPtr<SharedBuffer> serialize(const char *title, const char *mime,  MHTMLArchive::EncodingPolicy encodingPolicy)
    {
        return MHTMLArchive::generateMHTMLData(m_resources, encodingPolicy, title, mime);
    }
