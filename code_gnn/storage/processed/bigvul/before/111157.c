    static void finishOrCancel(WebPagePrivate* webPagePrivate)
    {
        webPagePrivate->*isActive = false;
    }
