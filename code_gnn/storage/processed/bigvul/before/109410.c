    virtual void didOpenFileSystem(const WebKit::WebString& name, const WebKit::WebURL& rootURL)
    {
        ASSERT_NOT_REACHED();
        delete this;
    }
