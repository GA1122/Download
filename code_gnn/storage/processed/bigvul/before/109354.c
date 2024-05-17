    virtual void didFail(const ResourceError&)
    {
        m_callback->sendFailure("Loading resource for inspector failed");
        dispose();
    }
