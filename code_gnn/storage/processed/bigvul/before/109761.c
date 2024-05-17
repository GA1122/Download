void Document::processHttpEquiv(const String& equiv, const String& content)
{
    ASSERT(!equiv.isNull() && !content.isNull());

    if (equalIgnoringCase(equiv, "default-style"))
        processHttpEquivDefaultStyle(content);
    else if (equalIgnoringCase(equiv, "refresh"))
        processHttpEquivRefresh(content);
    else if (equalIgnoringCase(equiv, "set-cookie"))
        processHttpEquivSetCookie(content);
    else if (equalIgnoringCase(equiv, "content-language"))
        setContentLanguage(content);
    else if (equalIgnoringCase(equiv, "x-dns-prefetch-control"))
        parseDNSPrefetchControlHeader(content);
    else if (equalIgnoringCase(equiv, "x-frame-options"))
        processHttpEquivXFrameOptions(content);
    else if (equalIgnoringCase(equiv, "content-security-policy")
        || equalIgnoringCase(equiv, "content-security-policy-report-only")
        || equalIgnoringCase(equiv, "x-webkit-csp")
        || equalIgnoringCase(equiv, "x-webkit-csp-report-only"))
        processHttpEquivContentSecurityPolicy(equiv, content);
}
