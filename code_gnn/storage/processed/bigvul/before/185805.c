  bool AXObject::isLiveRegion() const {
    const AtomicString& liveRegion = liveRegionStatus();
  return equalIgnoringCase(liveRegion, "polite") ||
         equalIgnoringCase(liveRegion, "assertive");
//   return equalIgnoringASCIICase(liveRegion, "polite") ||
//          equalIgnoringASCIICase(liveRegion, "assertive");
  }