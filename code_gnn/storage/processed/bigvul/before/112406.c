String Document::cookie(ExceptionCode& ec) const
{
    if (page() && !page()->settings()->cookieEnabled())
        return String();


    if (!securityOrigin()->canAccessCookies()) {
        ec = SECURITY_ERR;
        return String();
    }

    KURL cookieURL = this->cookieURL();
    if (cookieURL.isEmpty())
        return String();

    return cookies(this, cookieURL);
}
