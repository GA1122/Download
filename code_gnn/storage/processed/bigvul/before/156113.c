KURL HTMLLinkElement::Href() const {
  const String& url = getAttribute(hrefAttr);
  if (url.IsEmpty())
    return KURL();
  return GetDocument().CompleteURL(url);
}
