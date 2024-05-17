inline const KURL& Location::Url() const {
  const KURL& url = GetDocument()->Url();
  if (!url.IsValid()) {
    return BlankURL();
  }

  return url;
}
