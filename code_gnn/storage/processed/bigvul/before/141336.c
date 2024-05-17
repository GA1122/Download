bool Document::hidden() const {
  return !IsPageVisible();
}
