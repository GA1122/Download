    virtual void willSendRequest(WebFrame* frame, unsigned, WebURLRequest& request, const WebURLResponse&)
    {
        m_policy = request.cachePolicy();
        m_willSendRequestCallCount++;
    }
