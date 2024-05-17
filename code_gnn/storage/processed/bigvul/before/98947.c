KURL HTMLLinkElement::href() const
{
    return document()->completeURL(getAttribute(hrefAttr));
}
