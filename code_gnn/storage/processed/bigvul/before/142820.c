bool HTMLMediaElement::IsHLSURL(const KURL& url) {
  if (url.IsNull() || url.IsEmpty())
    return false;

  if (!url.IsLocalFile() && !url.ProtocolIs("http") && !url.ProtocolIs("https"))
    return false;

  return url.GetString().Contains("m3u8");
}
