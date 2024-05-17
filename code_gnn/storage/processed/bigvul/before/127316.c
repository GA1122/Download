    virtual WebFrame* createChildFrame(WebFrame*, const WebString&)
    {
        m_childFrameCreationCount++;
        return WebFrame::create(m_client);
    }
