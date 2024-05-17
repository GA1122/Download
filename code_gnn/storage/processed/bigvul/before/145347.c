void HTMLAnchorElement::setURL(const KURL& url)
{
    setHref(AtomicString(url.getString()));
}
