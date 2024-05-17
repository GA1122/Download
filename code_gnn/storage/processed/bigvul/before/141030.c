bool Document::DispatchBeforeUnloadEvent(ChromeClient* chrome_client,
                                         bool is_reload,
                                         bool& did_allow_navigation) {
  if (!dom_window_)
    return true;

  if (!body())
    return true;

  if (ProcessingBeforeUnload())
    return false;

  BeforeUnloadEvent& before_unload_event = *BeforeUnloadEvent::Create();
  before_unload_event.initEvent(event_type_names::kBeforeunload, false, true);
  load_event_progress_ = kBeforeUnloadEventInProgress;
  const TimeTicks beforeunload_event_start = CurrentTimeTicks();
  dom_window_->DispatchEvent(before_unload_event, this);
  const TimeTicks beforeunload_event_end = CurrentTimeTicks();
  load_event_progress_ = kBeforeUnloadEventCompleted;
  DEFINE_STATIC_LOCAL(
      CustomCountHistogram, beforeunload_histogram,
      ("DocumentEventTiming.BeforeUnloadDuration", 0, 10000000, 50));
  beforeunload_histogram.CountMicroseconds(beforeunload_event_end -
                                           beforeunload_event_start);
  if (!before_unload_event.defaultPrevented())
    DefaultEventHandler(before_unload_event);

  enum BeforeUnloadDialogHistogramEnum {
    kNoDialogNoText,
    kNoDialogNoUserGesture,
    kNoDialogMultipleConfirmationForNavigation,
    kShowDialog,
    kNoDialogAutoCancelTrue,
    kDialogEnumMax
  };
  DEFINE_STATIC_LOCAL(EnumerationHistogram, beforeunload_dialog_histogram,
                      ("Document.BeforeUnloadDialog", kDialogEnumMax));
  if (before_unload_event.returnValue().IsNull()) {
    beforeunload_dialog_histogram.Count(kNoDialogNoText);
  }
  if (!GetFrame() || before_unload_event.returnValue().IsNull())
    return true;

  if (!GetFrame()->HasBeenActivated()) {
    beforeunload_dialog_histogram.Count(kNoDialogNoUserGesture);
    String message =
        "Blocked attempt to show a 'beforeunload' confirmation panel for a "
        "frame that never had a user gesture since its load. "
        "https://www.chromestatus.com/feature/5082396709879808";
    Intervention::GenerateReport(frame_, "BeforeUnloadNoGesture", message);
    return true;
  }

  if (did_allow_navigation) {
    beforeunload_dialog_histogram.Count(
        kNoDialogMultipleConfirmationForNavigation);
    String message =
        "Blocked attempt to show multiple 'beforeunload' confirmation panels "
        "for a single navigation.";
    Intervention::GenerateReport(frame_, "BeforeUnloadMultiple", message);
    return true;
  }

  if (!chrome_client) {
    beforeunload_dialog_histogram.Count(kNoDialogAutoCancelTrue);
    did_allow_navigation = false;
    return false;
  }

  String text = before_unload_event.returnValue();
  beforeunload_dialog_histogram.Count(
      BeforeUnloadDialogHistogramEnum::kShowDialog);
  const TimeTicks beforeunload_confirmpanel_start = CurrentTimeTicks();
  did_allow_navigation =
      chrome_client->OpenBeforeUnloadConfirmPanel(text, frame_, is_reload);
  const TimeTicks beforeunload_confirmpanel_end = CurrentTimeTicks();
  if (did_allow_navigation) {
    UMA_HISTOGRAM_MEDIUM_TIMES(
        "DocumentEventTiming.BeforeUnloadDialogDuration.ByNavigation",
        beforeunload_confirmpanel_end - beforeunload_confirmpanel_start);
    return true;
  }

  return false;
}
