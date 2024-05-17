bool HTMLMediaElement::hasClosedCaptions() const {
  if (!m_textTracks)
    return false;

  for (unsigned i = 0; i < m_textTracks->length(); ++i) {
    if (m_textTracks->anonymousIndexedGetter(i)->canBeRendered())
      return true;
  }

  return false;
}
