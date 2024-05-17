    void writeDOMFileSystem(int type, const String& name, const String& url)
    {
        append(DOMFileSystemTag);
        doWriteUint32(type);
        doWriteWebCoreString(name);
        doWriteWebCoreString(url);
    }
