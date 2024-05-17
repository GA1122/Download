    virtual void didReadMetadata(const WebKit::WebFileInfo& info)
    {
        ASSERT(m_callbacks);
        if (info.type != WebKit::WebFileInfo::TypeFile || info.length < 0)
            m_callbacks->didFail(WebKit::WebFileErrorInvalidState);
        else {
            OwnPtr<AsyncFileWriterChromium> asyncFileWriterChromium = adoptPtr(new AsyncFileWriterChromium(m_client));
            OwnPtr<WebKit::WebFileWriter> webFileWriter = adoptPtr(m_webFileSystem->createFileWriter(m_path, asyncFileWriterChromium.get()));
            asyncFileWriterChromium->setWebFileWriter(webFileWriter.release());
            m_callbacks->didCreateFileWriter(asyncFileWriterChromium.release(), info.length);
        }
        delete this;
    }
