bool AXNodeObject::isAnchor() const {
  return !isNativeImage() && isLink();
}
