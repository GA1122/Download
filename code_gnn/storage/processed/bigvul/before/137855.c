void MediaControlPanelElement::didBecomeVisible() {
  DCHECK(m_isDisplayed && m_opaque);
  mediaElement().mediaControlsDidBecomeVisible();
}
