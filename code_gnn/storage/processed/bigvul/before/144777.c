bool TabLifecycleUnitSource::TabLifecycleUnit::DiscardTab() {
  return Discard(DiscardReason::kExternal);
}
