void LocalDOMWindow::EnqueuePageshowEvent(PageshowEventPersistence persisted) {
  if (ScopedEventQueue::Instance()->ShouldQueueEvents() && document_) {
      EnqueueWindowEvent(
        PageTransitionEvent::Create(EventTypeNames::pageshow, persisted));
    return;
  }
  DispatchEvent(
      PageTransitionEvent::Create(EventTypeNames::pageshow, persisted),
      document_.Get());
}
