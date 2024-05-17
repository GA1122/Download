AXObject::AXObjectVector AXNodeObject::radioButtonsInGroup() const {
  AXObjectVector radioButtons;
  if (!m_node || roleValue() != RadioButtonRole)
    return radioButtons;

  if (isHTMLInputElement(m_node)) {
    HTMLInputElement* radioButton = toHTMLInputElement(m_node);
    HeapVector<Member<HTMLInputElement>> htmlRadioButtons =
        findAllRadioButtonsWithSameName(radioButton);
    for (size_t i = 0; i < htmlRadioButtons.size(); ++i) {
      AXObject* axRadioButton =
          axObjectCache().getOrCreate(htmlRadioButtons[i]);
      if (axRadioButton)
        radioButtons.push_back(axRadioButton);
    }
    return radioButtons;
  }

  AXObject* parent = parentObject();
  if (parent && parent->roleValue() == RadioGroupRole) {
    for (size_t i = 0; i < parent->children().size(); ++i) {
      AXObject* child = parent->children()[i];
      DCHECK(child);
      if (child->roleValue() == RadioButtonRole &&
          !child->accessibilityIsIgnored()) {
        radioButtons.push_back(child);
      }
    }
  }

  return radioButtons;
}
