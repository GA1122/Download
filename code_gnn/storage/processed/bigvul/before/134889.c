    static void printWebURLs(const WebVector<WebURL>& urls)
    {
        for (size_t i = 0; i < urls.size(); i++)
            printf("%s\n", urls[i].spec().data());
    }
