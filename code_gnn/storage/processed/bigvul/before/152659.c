void HTMLFormControlElement::parseAttribute(const QualifiedName& name,
                                            const AtomicString& oldValue,
                                            const AtomicString& value) {
  if (name == formAttr) {
    formAttributeChanged();
    UseCounter::count(document(), UseCounter::FormAttribute);
  } else if (name == disabledAttr) {
    if (oldValue.isNull() != value.isNull())
      disabledAttributeChanged();
  } else if (name == readonlyAttr) {
    if (oldValue.isNull() != value.isNull()) {
      setNeedsWillValidateCheck();
      pseudoStateChanged(CSSSelector::PseudoReadOnly);
      pseudoStateChanged(CSSSelector::PseudoReadWrite);
      if (layoutObject())
        LayoutTheme::theme().controlStateChanged(*layoutObject(),
                                                 ReadOnlyControlState);
    }
  } else if (name == requiredAttr) {
    if (oldValue.isNull() != value.isNull())
      requiredAttributeChanged();
    UseCounter::count(document(), UseCounter::RequiredAttribute);
  } else if (name == autofocusAttr) {
    HTMLElement::parseAttribute(name, oldValue, value);
    UseCounter::count(document(), UseCounter::AutoFocusAttribute);
  } else {
    HTMLElement::parseAttribute(name, oldValue, value);
  }
}
