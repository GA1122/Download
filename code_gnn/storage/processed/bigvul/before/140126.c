bool HTMLMediaElement::supportsFocus() const {
  if (ownerDocument()->isMediaDocument())
    return false;

  return shouldShowControls() || HTMLElement::supportsFocus();
}
