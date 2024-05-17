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
  dom_window_->DispatchEvent(before_unload_event, this);
  load_event_progress_ = kBeforeUnloadEventCompleted;
  if (!before_unload_event->defaultPrevented())
    DefaultEventHandler(before_unload_event);
  if (!GetFrame() || before_unload_event->returnValue().IsNull())
    return true;

  if (!GetFrame()->HasReceivedUserGesture()) {
    AddConsoleMessage(ConsoleMessage::Create(
        kJSMessageSource, kErrorMessageLevel,
        "Blocked attempt to show a 'beforeunload' confirmation panel for a "
        "frame that never had a user gesture since its load. "
        "https://www.chromestatus.com/feature/5082396709879808"));
    return true;
  }

  if (did_allow_navigation) {
    AddConsoleMessage(ConsoleMessage::Create(
        kJSMessageSource, kErrorMessageLevel,
        "Blocked attempt to show multiple 'beforeunload' confirmation panels "
        "for a single navigation."));
    return true;
  }

  String text = before_unload_event->returnValue();
  if (chrome_client.OpenBeforeUnloadConfirmPanel(text, frame_, is_reload)) {
    did_allow_navigation = true;
    return true;
  }
  return false;
}
