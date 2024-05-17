void MediaControlPanelElement::makeTransparent() {
  if (!m_opaque)
    return;

  setInlineStyleProperty(CSSPropertyOpacity, 0.0,
                         CSSPrimitiveValue::UnitType::Number);

  m_opaque = false;
  startTimer();
}
