bool Document::hasManifest() const
{
    return documentElement() && isHTMLHtmlElement(documentElement()) && documentElement()->hasAttribute(manifestAttr);
}
