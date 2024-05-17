void AXLayoutObject::addInlineTextBoxChildren(bool force) {
  Settings* settings = getDocument()->settings();
  if (!force &&
      (!settings || !settings->getInlineTextBoxAccessibilityEnabled()))
    return;

  if (!getLayoutObject() || !getLayoutObject()->isText())
    return;

  if (getLayoutObject()->needsLayout()) {
    return;
  }

  LayoutText* layoutText = toLayoutText(getLayoutObject());
  for (RefPtr<AbstractInlineTextBox> box =
           layoutText->firstAbstractInlineTextBox();
       box.get(); box = box->nextInlineTextBox()) {
    AXObject* axObject = axObjectCache().getOrCreate(box.get());
    if (!axObject->accessibilityIsIgnored())
      m_children.push_back(axObject);
  }
}
