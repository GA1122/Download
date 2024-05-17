bool TabStripGtk::IsAnimating() const {
  return active_animation_.get() != NULL;
}
