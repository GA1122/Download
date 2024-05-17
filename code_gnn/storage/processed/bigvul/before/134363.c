bool NewTabButton::HasHitTestMask() const {
  return !tab_strip_->SizeTabButtonToTopOfTabStrip();
}
