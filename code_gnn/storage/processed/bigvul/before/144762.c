    ShouldIgnoreFeatureUsageEvent() {
  if (!writer_)
    return true;

  if (TabLoadTracker::Get()->GetLoadingState(web_contents()) !=
      LoadingState::LOADED) {
    return true;
  }

  return false;
}
