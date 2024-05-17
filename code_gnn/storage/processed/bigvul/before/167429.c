bool HTMLStyleElement::disabled() const {
  if (!sheet_)
    return false;

  return sheet_->disabled();
}
