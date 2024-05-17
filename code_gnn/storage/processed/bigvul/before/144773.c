bool TabLifecycleUnitSource::TabLifecycleUnit::CanPurge() const {
  return !IsMediaTab();
}
