void HTMLMediaElement::closeMediaSource() {
  if (!m_mediaSource)
    return;

  m_mediaSource->close();
  m_mediaSource = nullptr;
}
