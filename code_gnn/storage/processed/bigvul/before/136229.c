    const SerializedResource* getResource(const char* url, const char* mimeType)
    {
        KURL kURL = KURL(m_baseUrl, url);
        String mime(mimeType);
        for (size_t i = 0; i < m_resources.size(); ++i) {
            const SerializedResource& resource = m_resources[i];
            if (resource.url == kURL && !resource.data->isEmpty()
                && (mime.isNull() || equalIgnoringCase(resource.mimeType, mime)))
                return &resource;
        }
        return nullptr;
    }
