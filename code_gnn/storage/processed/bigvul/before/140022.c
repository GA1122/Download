bool HTMLMediaElement::loadIsDeferred() const {
  return m_deferredLoadState != NotDeferred;
}
