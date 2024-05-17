void HTMLMediaElement::loadSourceFromObject() {
  DCHECK(m_srcObject);
  m_loadState = LoadingFromSrcObject;

  loadResource(WebMediaPlayerSource(WebMediaStream(m_srcObject)),
               ContentType((String())));
}
