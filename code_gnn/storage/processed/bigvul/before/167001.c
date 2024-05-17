void CSSStyleSheet::setDisabled(bool disabled) {
  if (disabled == is_disabled_)
    return;
  is_disabled_ = disabled;

  DidMutate();
}
