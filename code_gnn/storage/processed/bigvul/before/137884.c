bool MediaControlDownloadButtonElement::shouldDisplayDownloadButton() {
  const KURL& url = mediaElement().currentSrc();

  if (document().page() && document().page()->settings().getHideDownloadUI())
    return false;

  if (url.isNull() || url.isEmpty())
    return false;

  if (url.isLocalFile() || url.protocolIs("blob"))
    return false;

  if (HTMLMediaElement::isMediaStreamURL(url.getString()))
    return false;

  if (HTMLMediaSource::lookup(url))
    return false;

  if (HTMLMediaElement::isHLSURL(url))
    return false;

  return true;
}
