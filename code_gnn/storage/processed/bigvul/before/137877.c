void MediaControlPanelElement::setIsDisplayed(bool isDisplayed) {
  if (m_isDisplayed == isDisplayed)
    return;

  m_isDisplayed = isDisplayed;
  if (m_isDisplayed && m_opaque)
    didBecomeVisible();
}
