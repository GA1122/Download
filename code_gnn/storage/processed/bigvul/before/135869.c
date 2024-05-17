const AtomicString& TextTrack::DisabledKeyword() {
  DEFINE_STATIC_LOCAL(const AtomicString, disabled, ("disabled"));
  return disabled;
}
