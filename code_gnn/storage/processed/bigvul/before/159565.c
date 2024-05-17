void Document::FinishedParsing() {
  DCHECK(!GetScriptableDocumentParser() || !parser_->IsParsing());
  DCHECK(!GetScriptableDocumentParser() || ready_state_ != kLoading);
  SetParsingState(kInDOMContentLoaded);
  DocumentParserTiming::From(*this).MarkParserStop();

  if (!document_timing_.DomContentLoadedEventStart())
    document_timing_.MarkDomContentLoadedEventStart();
  DispatchEvent(Event::CreateBubble(EventTypeNames::DOMContentLoaded));
  if (!document_timing_.DomContentLoadedEventEnd())
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
                         InspectorMarkLoadEvent::Data(frame));
    probe::domContentLoadedEventFired(frame);
    frame->GetIdlenessDetector()->DomContentLoadedEventFired();
  }

  element_data_cache_clear_timer_.StartOneShot(10, BLINK_FROM_HERE);

  fetcher_->ClearPreloads(ResourceFetcher::kClearSpeculativeMarkupPreloads);
  if (!frame_ || frame_->GetSettings()->GetSavePreviousDocumentResources() ==
                     SavePreviousDocumentResources::kUntilOnDOMContentLoaded) {
    fetcher_->ClearResourcesFromPreviousFetcher();
  }

  if (IsPrefetchOnly())
    WebPrerenderingSupport::Current()->PrefetchFinished();
}
