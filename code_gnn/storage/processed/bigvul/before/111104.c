WebCookieJar* WebPage::cookieJar() const
{
    if (!d->m_cookieJar)
        d->m_cookieJar = new WebCookieJar();

    return d->m_cookieJar;
}
