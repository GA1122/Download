void Document::processHttpEquivSetCookie(const AtomicString& content)
{
    if (!isHTMLDocument())
        return;

    toHTMLDocument(this)->setCookie(content, IGNORE_EXCEPTION);
}
