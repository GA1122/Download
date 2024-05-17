void HTMLMediaElement::requestReload(const WebURL& newUrl) {
  DCHECK(webMediaPlayer());
  DCHECK(!m_srcObject);
  DCHECK(newUrl.isValid());
  DCHECK(isSafeToLoadURL(newUrl, Complain));
  resetMediaPlayerAndMediaSource();
  startPlayerLoad(newUrl);
}
