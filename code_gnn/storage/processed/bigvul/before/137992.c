void AXLayoutObject::textChanged() {
  if (!m_layoutObject)
    return;

  Settings* settings = getDocument()->settings();
  if (settings && settings->getInlineTextBoxAccessibilityEnabled() &&
      roleValue() == StaticTextRole)
    childrenChanged();

  AXNodeObject::textChanged();
}
