bool HTMLLinkElement::ShouldLoadLink() {
  const KURL& href = GetNonEmptyURLAttribute(hrefAttr);
  return (IsInDocumentTree() ||
          (isConnected() && rel_attribute_.IsStyleSheet())) &&
         !href.PotentiallyDanglingMarkup();
}
