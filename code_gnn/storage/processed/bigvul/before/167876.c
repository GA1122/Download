bool Document::DispatchBeforeUnloadEvent(ChromeClient& chrome_client,
                                         bool is_reload,
                                         bool& did_allow_navigation) {
  if (!dom_window_)
    return true;

  if (!body())
    return true;

  if (ProcessingBeforeUnload())
    return false;

  BeforeUnloadEvent* before_unload_event = BeforeUnloadEvent::Create();
  before_unload_event->initEvent(EventTypeNames::beforeunload, false, true);
  load_event_progress_ = kBeforeUnloadEventInProgress;
  const TimeTicks beforeunload_event_start = CurrentTimeTicks();
  dom_window_->DispatchEvent(before_unload_event, this);
  const TimeTicks beforeunload_event_end = CurrentTimeTicks();
  load_event_progress_ = kBeforeUnloadEventCompleted;
  DEFINE_STATIC_LOCAL(
      CustomCountHistogram, beforeunload_histogram,
      ("DocumentEventTiming.BeforeUnloadDuration", 0, 10000000, 50));
  beforeunload_histogram.Count(
      (beforeunload_event_end - beforeunload_event_start).InMicroseconds());
  if (!before_unload_event->defaultPrevented())
    DefaultEventHandler(before_unload_event);

  enum BeforeUnloadDialogHistogramEnum {
    kNoDialogNoText,
    kNoDialogNoUserGesture,
    kNoDialogMultipleConfirmationForNavigation,
    kShowDialog,
    kDialogEnumMax
  };
  DEFINE_STATIC_LOCAL(EnumerationHistogram, beforeunload_dialog_histogram,
                      ("Document.BeforeUnloadDialog", kDialogEnumMax));
  if (before_unload_event->returnValue().IsNull()) {
    beforeunload_dialog_histogram.Count(kNoDialogNoText);
  }
  if (!GetFrame() || before_unload_event->returnValue().IsNull())
    return true;

  if (!GetFrame()->HasBeenActivated()) {
    beforeunload_dialog_histogram.Count(kNoDialogNoUserGesture);
    AddConsoleMessage(ConsoleMessage::Create(
        kInterventionMessageSource, kErrorMessageLevel,
        "Blocked attempt to show a 'beforeunload' confirmation panel for a "
        "frame that never had a user gesture since its load. "
        "https://www.chromestatus.com/feature/5082396709879808"));
    return true;
  }

  if (did_allow_navigation) {
    beforeunload_dialog_histogram.Count(
        kNoDialogMultipleConfirmationForNavigation);
    AddConsoleMessage(ConsoleMessage::Create(
        kInterventionMessageSource, kErrorMessageLevel,
        "Blocked attempt to show multiple 'beforeunload' confirmation panels "
        "for a single navigation."));
    return true;
  }
  String text = before_unload_event->returnValue();
  beforeunload_dialog_histogram.Count(
      BeforeUnloadDialogHistogramEnum::kShowDialog);
  if (chrome_client.OpenBeforeUnloadConfirmPanel(text, frame_, is_reload)) {
    did_allow_navigation = true;
    return true;
  }

  return false;
}
