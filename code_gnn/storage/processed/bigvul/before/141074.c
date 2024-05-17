void Document::FinishedParsing() {
  DCHECK(!GetScriptableDocumentParser() || !parser_->IsParsing());
  DCHECK(!GetScriptableDocumentParser() || ready_state_ != kLoading);
  SetParsingState(kInDOMContentLoaded);
  DocumentParserTiming::From(*this).MarkParserStop();

  if (document_timing_.DomContentLoadedEventStart().is_null())
    document_timing_.MarkDomContentLoadedEventStart();
  DispatchEvent(*Event::CreateBubble(event_type_names::kDOMContentLoaded));
  if (document_timing_.DomContentLoadedEventEnd().is_null())
    document_timing_.MarkDomContentLoadedEventEnd();
  SetParsingState(kFinishedParsing);

  Microtask::PerformCheckpoint(V8PerIsolateData::MainThreadIsolate());

  ScriptableDocumentParser* parser = GetScriptableDocumentParser();
  well_formed_ = parser && parser->WellFormed();

  if (LocalFrame* frame = GetFrame()) {
    if (title_.IsEmpty())
      DispatchDidReceiveTitle();

    const bool main_resource_was_already_requested =
        frame->Loader().StateMachine()->CommittedFirstRealDocumentLoad();

    if (main_resource_was_already_requested)
      UpdateStyleAndLayoutTree();

    BeginLifecycleUpdatesIfRenderingReady();

    frame->Loader().FinishedParsing();

    TRACE_EVENT_INSTANT1("devtools.timeline", "MarkDOMContent",
                         TRACE_EVENT_SCOPE_THREAD, "data",
                         inspector_mark_load_event::Data(frame));
    probe::DomContentLoadedEventFired(frame);
    frame->GetIdlenessDetector()->DomContentLoadedEventFired();

    if (auto* frame_coordinator = frame->GetFrameResourceCoordinator()) {
      SetInitialInterventionPolicies(frame_coordinator, this);
    }
  }

  element_data_cache_clear_timer_.StartOneShot(TimeDelta::FromSeconds(10),
                                               FROM_HERE);

  fetcher_->ClearPreloads(ResourceFetcher::kClearSpeculativeMarkupPreloads);

  if (IsPrefetchOnly())
    WebPrerenderingSupport::Current()->PrefetchFinished();
}
