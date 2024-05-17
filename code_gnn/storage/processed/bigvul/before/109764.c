void Document::processHttpEquivSetCookie(const String& content)
{
    if (!isHTMLDocument())
        return;

    toHTMLDocument(this)->setCookie(content, IGNORE_EXCEPTION);
}
