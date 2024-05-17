void HTMLInputElement::setIndeterminate(bool new_value) {
  if (indeterminate() == new_value)
    return;

  is_indeterminate_ = new_value;

  PseudoStateChanged(CSSSelector::kPseudoIndeterminate);

  if (LayoutObject* o = GetLayoutObject())
    o->InvalidateIfControlStateChanged(kCheckedControlState);
}
