void Document::setCookie(const String& value, ExceptionState& es)
{
    if (settings() && !settings()->cookieEnabled())
        return;


    if (!securityOrigin()->canAccessCookies()) {
        String accessDeniedMessage = "Access to 'cookie' is denied for this document.";
        if (isSandboxed(SandboxOrigin))
            es.throwSecurityError(accessDeniedMessage + " The document is sandboxed and lacks the 'allow-same-origin' flag.");
        else if (url().protocolIs("data"))
            es.throwSecurityError(accessDeniedMessage + " Cookies are disabled inside 'data:' URLs.");
        else
            es.throwSecurityError(accessDeniedMessage);
        return;
    }

    KURL cookieURL = this->cookieURL();
    if (cookieURL.isEmpty())
        return;

    setCookies(this, cookieURL, value);
}
