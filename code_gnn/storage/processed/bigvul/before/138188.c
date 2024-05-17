void AXObject::notifyIfIgnoredValueChanged() {
  bool isIgnored = accessibilityIsIgnored();
  if (lastKnownIsIgnoredValue() != isIgnored) {
    axObjectCache().childrenChanged(parentObject());
    setLastKnownIsIgnoredValue(isIgnored);
  }
}
