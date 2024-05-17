    virtual void didFail(WebKit::WebFileError error)
    {
        ASSERT(m_callbacks);
        m_callbacks->didFail(error);
        delete this;
    }
