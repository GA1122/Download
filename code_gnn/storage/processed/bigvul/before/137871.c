void MediaControlPanelElement::makeOpaque() {
  if (m_opaque)
    return;

  setInlineStyleProperty(CSSPropertyOpacity, 1.0,
                         CSSPrimitiveValue::UnitType::Number);
  m_opaque = true;

  if (m_isDisplayed) {
    setIsWanted(true);
    didBecomeVisible();
  }
}
