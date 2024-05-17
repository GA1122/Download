    void writeWebCoreString(const String& string)
    {
        append(StringTag);
        doWriteWebCoreString(string);
    }
