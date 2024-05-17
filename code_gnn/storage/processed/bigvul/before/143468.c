    void test(PreconnectTestCase testCase)
    {
        MockHTMLResourcePreloader preloader;
        KURL baseURL(ParsedURLString, testCase.baseURL);
        m_scanner->appendToEnd(String(testCase.inputHTML));
        m_scanner->scanAndPreload(&preloader, baseURL, nullptr);
        preloader.preconnectRequestVerification(testCase.preconnectedHost, testCase.crossOrigin);
    }
