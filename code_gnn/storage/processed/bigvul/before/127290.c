    void addResource(const char* url, const char* mime, const char* fileName)
    {
        addResource(url, mime, readFile(fileName));
    }
