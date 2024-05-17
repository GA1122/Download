void AXObject::setLastKnownIsIgnoredValue(bool isIgnored) {
  m_lastKnownIsIgnoredValue = isIgnored ? IgnoreObject : IncludeObject;
}
