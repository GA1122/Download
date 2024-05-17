bool AXObject::supportsRangeValue() const {
  return isProgressIndicator() || isMeter() || isSlider() || isScrollbar() ||
         isSpinButton();
}
