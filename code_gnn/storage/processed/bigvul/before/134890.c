    void registerMockedURLLoad(const std::string& url, const WebString& fileName)
    {
        URLTestHelpers::registerMockedURLLoad(toKURL(url), fileName, WebString::fromUTF8("pageserialization/"), WebString::fromUTF8("text/html"));
    }
