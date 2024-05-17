    virtual void didFailRedirectCheck()
    {
        m_callback->sendFailure("Loading resource for inspector failed redirect check");
        dispose();
    }
