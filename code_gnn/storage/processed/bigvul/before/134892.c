    static bool webVectorContains(const WebVector<WebURL>& vector, const char* url)
    {
        return vector.contains(WebURL(toKURL(std::string(url))));
    }
