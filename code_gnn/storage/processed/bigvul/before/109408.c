    virtual void didCreateSnapshotFile(const WebKit::WebFileInfo& info)
    {
        ASSERT_NOT_REACHED();
        delete this;
    }
