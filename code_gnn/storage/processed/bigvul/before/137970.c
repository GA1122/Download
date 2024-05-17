const AtomicString& AXLayoutObject::liveRegionStatus() const {
  DEFINE_STATIC_LOCAL(const AtomicString, liveRegionStatusAssertive,
                      ("assertive"));
  DEFINE_STATIC_LOCAL(const AtomicString, liveRegionStatusPolite, ("polite"));
  DEFINE_STATIC_LOCAL(const AtomicString, liveRegionStatusOff, ("off"));

  const AtomicString& liveRegionStatus =
      getAOMPropertyOrARIAAttribute(AOMStringProperty::kLive);
  if (liveRegionStatus.isEmpty()) {
    switch (roleValue()) {
      case AlertDialogRole:
      case AlertRole:
        return liveRegionStatusAssertive;
      case LogRole:
      case StatusRole:
        return liveRegionStatusPolite;
      case TimerRole:
      case MarqueeRole:
        return liveRegionStatusOff;
      default:
        break;
    }
  }

  return liveRegionStatus;
}
