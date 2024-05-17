void LocalDOMWindow::Trace(blink::Visitor* visitor) {
  visitor->Trace(document_);
  visitor->Trace(screen_);
  visitor->Trace(history_);
  visitor->Trace(locationbar_);
  visitor->Trace(menubar_);
  visitor->Trace(personalbar_);
  visitor->Trace(scrollbars_);
  visitor->Trace(statusbar_);
  visitor->Trace(toolbar_);
  visitor->Trace(navigator_);
  visitor->Trace(media_);
  visitor->Trace(custom_elements_);
  visitor->Trace(modulator_);
  visitor->Trace(external_);
  visitor->Trace(application_cache_);
  visitor->Trace(event_queue_);
  visitor->Trace(post_message_timers_);
  visitor->Trace(visualViewport_);
  visitor->Trace(event_listener_observers_);
  DOMWindow::Trace(visitor);
  Supplementable<LocalDOMWindow>::Trace(visitor);
}