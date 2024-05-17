String Document::cookie(ExceptionState& exceptionState) const
{
    if (settings() && !settings()->cookieEnabled())
        return String();


    if (!getSecurityOrigin()->canAccessCookies()) {
        if (isSandboxed(SandboxOrigin))
            exceptionState.throwSecurityError("The document is sandboxed and lacks the 'allow-same-origin' flag.");
        else if (url().protocolIs("data"))
            exceptionState.throwSecurityError("Cookies are disabled inside 'data:' URLs.");
        else
            exceptionState.throwSecurityError("Access is denied for this document.");
        return String();
    }

    if (getSecurityOrigin()->hasSuborigin() && !getSecurityOrigin()->suborigin()->policyContains(Suborigin::SuboriginPolicyOptions::UnsafeCookies))
        return String();

    KURL cookieURL = this->cookieURL();
    if (cookieURL.isEmpty())
        return String();

    return cookies(this, cookieURL);
}
