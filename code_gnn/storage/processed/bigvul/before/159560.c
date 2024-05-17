void Document::DispatchUnloadEvents() {
  PluginScriptForbiddenScope forbid_plugin_destructor_scripting;
  if (parser_)
    parser_->StopParsing();

  if (load_event_progress_ == kLoadEventNotRun)
    return;

  if (load_event_progress_ <= kUnloadEventInProgress) {
    Element* current_focused_element = FocusedElement();
    if (auto* input = ToHTMLInputElementOrNull(current_focused_element))
      input->EndEditing();
    if (load_event_progress_ < kPageHideInProgress) {
      load_event_progress_ = kPageHideInProgress;
      if (LocalDOMWindow* window = domWindow()) {
        const double pagehide_event_start = MonotonicallyIncreasingTime();
        window->DispatchEvent(
            PageTransitionEvent::Create(EventTypeNames::pagehide, false), this);
        const double pagehide_event_end = MonotonicallyIncreasingTime();
        DEFINE_STATIC_LOCAL(
            CustomCountHistogram, pagehide_histogram,
            ("DocumentEventTiming.PageHideDuration", 0, 10000000, 50));
        pagehide_histogram.Count((pagehide_event_end - pagehide_event_start) *
                                 1000000.0);
      }
      if (!frame_)
        return;

      mojom::PageVisibilityState visibility_state = GetPageVisibilityState();
      load_event_progress_ = kUnloadVisibilityChangeInProgress;
      if (visibility_state != mojom::PageVisibilityState::kHidden) {
        const double pagevisibility_hidden_event_start =
            MonotonicallyIncreasingTime();
        DispatchEvent(Event::CreateBubble(EventTypeNames::visibilitychange));
        const double pagevisibility_hidden_event_end =
            MonotonicallyIncreasingTime();
        DEFINE_STATIC_LOCAL(CustomCountHistogram, pagevisibility_histogram,
                            ("DocumentEventTiming.PageVibilityHiddenDuration",
                             0, 10000000, 50));
        pagevisibility_histogram.Count((pagevisibility_hidden_event_end -
                                        pagevisibility_hidden_event_start) *
                                       1000000.0);
        DispatchEvent(
            Event::CreateBubble(EventTypeNames::webkitvisibilitychange));
      }
      if (!frame_)
        return;

      DocumentLoader* document_loader =
          frame_->Loader().GetProvisionalDocumentLoader();
      load_event_progress_ = kUnloadEventInProgress;
      Event* unload_event(Event::Create(EventTypeNames::unload));
      if (document_loader && !document_loader->GetTiming().UnloadEventStart() &&
          !document_loader->GetTiming().UnloadEventEnd()) {
        DocumentLoadTiming& timing = document_loader->GetTiming();
        DCHECK(timing.NavigationStart());
        const double unload_event_start = MonotonicallyIncreasingTime();
        timing.MarkUnloadEventStart(unload_event_start);
        frame_->DomWindow()->DispatchEvent(unload_event, this);
        const double unload_event_end = MonotonicallyIncreasingTime();
        DEFINE_STATIC_LOCAL(
            CustomCountHistogram, unload_histogram,
            ("DocumentEventTiming.UnloadDuration", 0, 10000000, 50));
        unload_histogram.Count((unload_event_end - unload_event_start) *
                               1000000.0);
        timing.MarkUnloadEventEnd(unload_event_end);
      } else {
        frame_->DomWindow()->DispatchEvent(unload_event, frame_->GetDocument());
      }
    }
    load_event_progress_ = kUnloadEventHandled;
  }

  if (!frame_)
    return;

  bool keep_event_listeners =
      frame_->Loader().GetProvisionalDocumentLoader() &&
      frame_->ShouldReuseDefaultView(
          frame_->Loader().GetProvisionalDocumentLoader()->Url());
  if (!keep_event_listeners)
    RemoveAllEventListenersRecursively();
}
