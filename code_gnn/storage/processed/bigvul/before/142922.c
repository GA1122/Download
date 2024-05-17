bool HTMLMediaElement::SupportsFocus() const {
  if (ownerDocument()->IsMediaDocument())
    return false;

  return ShouldShowControls() || HTMLElement::SupportsFocus();
}
