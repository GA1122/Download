bool Document::HasAppCacheManifest() const {
  return IsHTMLHtmlElement(documentElement()) &&
         documentElement()->hasAttribute(manifestAttr);
}
