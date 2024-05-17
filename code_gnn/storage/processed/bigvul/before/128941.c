    void doWriteWebCoreString(const String& string)
    {
        StringUTF8Adaptor stringUTF8(string);
        doWriteString(stringUTF8.data(), stringUTF8.length());
    }
