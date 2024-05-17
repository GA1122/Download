void Document::processReferrerPolicy(const String& policy)
{
    ASSERT(!policy.isNull());

    if (equalIgnoringCase(policy, "no-referrer") || equalIgnoringCase(policy, "never"))
        setReferrerPolicy(ReferrerPolicyNever);
    else if (equalIgnoringCase(policy, "unsafe-url") || equalIgnoringCase(policy, "always"))
        setReferrerPolicy(ReferrerPolicyAlways);
    else if (equalIgnoringCase(policy, "origin"))
        setReferrerPolicy(ReferrerPolicyOrigin);
    else if (equalIgnoringCase(policy, "origin-when-cross-origin") || equalIgnoringCase(policy, "origin-when-crossorigin"))
        setReferrerPolicy(ReferrerPolicyOriginWhenCrossOrigin);
    else if (equalIgnoringCase(policy, "no-referrer-when-downgrade") || equalIgnoringCase(policy, "default"))
        setReferrerPolicy(ReferrerPolicyNoReferrerWhenDowngrade);
    else
        addConsoleMessage(ConsoleMessage::create(RenderingMessageSource, ErrorMessageLevel, "Failed to set referrer policy: The value '" + policy + "' is not one of 'always', 'default', 'never', 'no-referrer', 'no-referrer-when-downgrade', 'origin', 'origin-when-crossorigin', or 'unsafe-url'. This document's referrer policy has been left unchanged."));
}
