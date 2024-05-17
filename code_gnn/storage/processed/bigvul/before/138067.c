bool AXNodeObject::isImageButton() const {
  return isNativeImage() && isButton();
}
