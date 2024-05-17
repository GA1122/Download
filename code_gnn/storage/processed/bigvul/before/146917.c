String Document::visibilityState() const {
  return PageVisibilityStateString(GetPageVisibilityState());
}
