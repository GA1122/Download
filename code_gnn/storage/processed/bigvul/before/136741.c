void LocalDOMWindow::DispatchLoadEvent() {
  Event* load_event(Event::Create(EventTypeNames::load));
  DocumentLoader* document_loader =
      GetFrame() ? GetFrame()->Loader().GetDocumentLoader() : nullptr;
  if (document_loader &&
      document_loader->GetTiming().LoadEventStart().is_null()) {
    DocumentLoadTiming& timing = document_loader->GetTiming();
    timing.MarkLoadEventStart();
    DispatchEvent(load_event, document());
    timing.MarkLoadEventEnd();
    DCHECK(document_loader->Fetcher());
    if (GetFrame() &&
        document_loader == GetFrame()->Loader().GetDocumentLoader() &&
        document_loader->Fetcher()->CountPreloads()) {
      unused_preloads_timer_.StartOneShot(kUnusedPreloadTimeoutInSeconds,
                                          FROM_HERE);
    }
  } else {
    DispatchEvent(load_event, document());
  }

  if (GetFrame()) {
    WindowPerformance* performance = DOMWindowPerformance::performance(*this);
    DCHECK(performance);
    performance->NotifyNavigationTimingToObservers();
  }

  FrameOwner* owner = GetFrame() ? GetFrame()->Owner() : nullptr;
  if (owner)
    owner->DispatchLoad();

  TRACE_EVENT_INSTANT1("devtools.timeline", "MarkLoad",
                       TRACE_EVENT_SCOPE_THREAD, "data",
                       InspectorMarkLoadEvent::Data(GetFrame()));
  probe::loadEventFired(GetFrame());
}
