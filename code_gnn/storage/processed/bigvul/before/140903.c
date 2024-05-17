const AtomicString& connectionCloseReasonToString(
    WebPresentationConnectionCloseReason reason) {
  DEFINE_STATIC_LOCAL(const AtomicString, errorValue, ("error"));
  DEFINE_STATIC_LOCAL(const AtomicString, closedValue, ("closed"));
  DEFINE_STATIC_LOCAL(const AtomicString, wentAwayValue, ("wentaway"));

  switch (reason) {
    case WebPresentationConnectionCloseReason::Error:
      return errorValue;
    case WebPresentationConnectionCloseReason::Closed:
      return closedValue;
    case WebPresentationConnectionCloseReason::WentAway:
      return wentAwayValue;
  }

  ASSERT_NOT_REACHED();
  return errorValue;
}
