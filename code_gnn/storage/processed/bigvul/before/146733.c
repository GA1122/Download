void Document::DispatchUnloadEvents() {
  PluginScriptForbiddenScope forbid_plugin_destructor_scripting;
  if (parser_)
    parser_->StopParsing();

  if (load_event_progress_ == kLoadEventNotRun)
    return;

  if (load_event_progress_ <= kUnloadEventInProgress) {
    Element* current_focused_element = FocusedElement();
    if (isHTMLInputElement(current_focused_element))
      toHTMLInputElement(*current_focused_element).EndEditing();
    if (load_event_progress_ < kPageHideInProgress) {
      load_event_progress_ = kPageHideInProgress;
      if (LocalDOMWindow* window = domWindow())
        window->DispatchEvent(
            PageTransitionEvent::Create(EventTypeNames::pagehide, false), this);
      if (!frame_)
        return;

      PageVisibilityState visibility_state = GetPageVisibilityState();
      load_event_progress_ = kUnloadVisibilityChangeInProgress;
      if (visibility_state != kPageVisibilityStateHidden) {
        DispatchEvent(Event::CreateBubble(EventTypeNames::visibilitychange));
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
        timing.MarkUnloadEventStart();
        frame_->DomWindow()->DispatchEvent(unload_event, this);
        timing.MarkUnloadEventEnd();
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
