const AtomicString& AXLayoutObject::liveRegionRelevant() const {
  DEFINE_STATIC_LOCAL(const AtomicString, defaultLiveRegionRelevant,
                      ("additions text"));
  const AtomicString& relevant =
      getAOMPropertyOrARIAAttribute(AOMStringProperty::kRelevant);

  if (relevant.isEmpty())
    return defaultLiveRegionRelevant;

  return relevant;
}
