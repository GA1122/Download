bool Document::hasAppCacheManifest() const
{
    return isHTMLHtmlElement(documentElement()) && documentElement()->hasAttribute(manifestAttr);
}
