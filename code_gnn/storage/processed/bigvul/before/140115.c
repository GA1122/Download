bool HTMLMediaElement::shouldShowControls(
    const RecordMetricsBehavior recordMetrics) const {
  if (fastHasAttribute(controlsAttr)) {
    if (recordMetrics == RecordMetricsBehavior::DoRecord)
      showControlsHistogram().count(MediaControlsShowAttribute);
    return true;
  }

  if (isFullscreen()) {
    if (recordMetrics == RecordMetricsBehavior::DoRecord)
      showControlsHistogram().count(MediaControlsShowFullscreen);
    return true;
  }

  LocalFrame* frame = document().frame();
  if (frame && !document().canExecuteScripts(NotAboutToExecuteScript)) {
    if (recordMetrics == RecordMetricsBehavior::DoRecord)
      showControlsHistogram().count(MediaControlsShowNoScript);
    return true;
  }

  if (recordMetrics == RecordMetricsBehavior::DoRecord)
    showControlsHistogram().count(MediaControlsShowNotShown);
  return false;
}
