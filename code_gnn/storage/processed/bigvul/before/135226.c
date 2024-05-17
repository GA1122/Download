void Document::enforceStrictMixedContentChecking()
{
    securityContext().setShouldEnforceStrictMixedContentChecking(true);
    if (frame())
        frame()->loader().client()->didEnforceStrictMixedContentChecking();
}
