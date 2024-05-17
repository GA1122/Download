void Document::processHttpEquivContentSecurityPolicy(const String& equiv, const String& content)
{
    if (equalIgnoringCase(equiv, "content-security-policy"))
        contentSecurityPolicy()->didReceiveHeader(content, ContentSecurityPolicy::Enforce);
    else if (equalIgnoringCase(equiv, "content-security-policy-report-only"))
        contentSecurityPolicy()->didReceiveHeader(content, ContentSecurityPolicy::Report);
    else if (equalIgnoringCase(equiv, "x-webkit-csp"))
        UseCounter::countDeprecation(this, UseCounter::PrefixedContentSecurityPolicy);
    else if (equalIgnoringCase(equiv, "x-webkit-csp-report-only"))
        UseCounter::countDeprecation(this, UseCounter::PrefixedContentSecurityPolicyReportOnly);
    else
        ASSERT_NOT_REACHED();
}
