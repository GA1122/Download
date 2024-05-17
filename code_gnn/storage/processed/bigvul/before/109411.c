    virtual void didReadDirectory(const WebKit::WebVector<WebKit::WebFileSystemEntry>& entries, bool hasMore)
    {
        ASSERT_NOT_REACHED();
        delete this;
    }
