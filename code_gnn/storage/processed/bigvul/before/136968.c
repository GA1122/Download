void HTMLInputElement::SetShouldRevealPassword(bool value) {
  if (!!should_reveal_password_ == value)
    return;
  should_reveal_password_ = value;
  LazyReattachIfAttached();
}
