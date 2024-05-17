void HTMLStyleElement::DispatchPendingEvent(
    std::unique_ptr<IncrementLoadEventDelayCount> count) {
  if (loaded_sheet_) {
    if (GetDocument().HasListenerType(
            Document::kLoadListenerAtCapturePhaseOrAtStyleElement))
      DispatchEvent(Event::Create(EventTypeNames::load));
  } else {
    DispatchEvent(Event::Create(EventTypeNames::error));
  }
  count->ClearAndCheckLoadEvent();
}
