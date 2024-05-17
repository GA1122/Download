bool Document::HasAppCacheManifest() const {
  return isHTMLHtmlElement(documentElement()) &&
         documentElement()->hasAttribute(manifestAttr);
}
