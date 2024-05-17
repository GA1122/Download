bool HTMLMediaElement::isAutoplayingMuted() {
  if (!isHTMLVideoElement() ||
      !RuntimeEnabledFeatures::autoplayMutedVideosEnabled()) {
    return false;
  }

  return !paused() && muted() && isLockedPendingUserGesture();
}
