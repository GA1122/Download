void Document::processHttpEquivContentSecurityPolicy(const AtomicString& equiv, const AtomicString& content)
{
    if (importLoader())
        return;
    if (equalIgnoringCase(equiv, "content-security-policy"))
        contentSecurityPolicy()->didReceiveHeader(content, ContentSecurityPolicyHeaderTypeEnforce, ContentSecurityPolicyHeaderSourceMeta);
    else if (equalIgnoringCase(equiv, "content-security-policy-report-only"))
        contentSecurityPolicy()->didReceiveHeader(content, ContentSecurityPolicyHeaderTypeReport, ContentSecurityPolicyHeaderSourceMeta);
    else
        ASSERT_NOT_REACHED();
}
