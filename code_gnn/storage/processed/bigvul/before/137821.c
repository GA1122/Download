MediaControlPanelElement::MediaControlPanelElement(MediaControls& mediaControls)
    : MediaControlDivElement(mediaControls, MediaControlsPanel),
      m_isDisplayed(false),
      m_opaque(true),
      m_transitionTimer(this, &MediaControlPanelElement::transitionTimerFired) {
}
