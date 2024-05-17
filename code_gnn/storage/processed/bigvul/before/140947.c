void Document::AddListenerTypeIfNeeded(const AtomicString& event_type,
                                       EventTarget& event_target) {
  if (event_type == event_type_names::kDOMSubtreeModified) {
    UseCounter::Count(*this, WebFeature::kDOMSubtreeModifiedEvent);
    AddMutationEventListenerTypeIfEnabled(kDOMSubtreeModifiedListener);
  } else if (event_type == event_type_names::kDOMNodeInserted) {
    UseCounter::Count(*this, WebFeature::kDOMNodeInsertedEvent);
    AddMutationEventListenerTypeIfEnabled(kDOMNodeInsertedListener);
  } else if (event_type == event_type_names::kDOMNodeRemoved) {
    UseCounter::Count(*this, WebFeature::kDOMNodeRemovedEvent);
    AddMutationEventListenerTypeIfEnabled(kDOMNodeRemovedListener);
  } else if (event_type == event_type_names::kDOMNodeRemovedFromDocument) {
    UseCounter::Count(*this, WebFeature::kDOMNodeRemovedFromDocumentEvent);
    AddMutationEventListenerTypeIfEnabled(kDOMNodeRemovedFromDocumentListener);
  } else if (event_type == event_type_names::kDOMNodeInsertedIntoDocument) {
    UseCounter::Count(*this, WebFeature::kDOMNodeInsertedIntoDocumentEvent);
    AddMutationEventListenerTypeIfEnabled(kDOMNodeInsertedIntoDocumentListener);
  } else if (event_type == event_type_names::kDOMCharacterDataModified) {
    UseCounter::Count(*this, WebFeature::kDOMCharacterDataModifiedEvent);
    AddMutationEventListenerTypeIfEnabled(kDOMCharacterDataModifiedListener);
  } else if (event_type == event_type_names::kWebkitAnimationStart ||
             event_type == event_type_names::kAnimationstart) {
    AddListenerType(kAnimationStartListener);
  } else if (event_type == event_type_names::kWebkitAnimationEnd ||
             event_type == event_type_names::kAnimationend) {
    AddListenerType(kAnimationEndListener);
  } else if (event_type == event_type_names::kWebkitAnimationIteration ||
             event_type == event_type_names::kAnimationiteration) {
    AddListenerType(kAnimationIterationListener);
    if (View()) {
      View()->ScheduleAnimation();
    }
  } else if (event_type == event_type_names::kTransitioncancel) {
    AddListenerType(kTransitionCancelListener);
  } else if (event_type == event_type_names::kTransitionrun) {
    AddListenerType(kTransitionRunListener);
  } else if (event_type == event_type_names::kTransitionstart) {
    AddListenerType(kTransitionStartListener);
  } else if (event_type == event_type_names::kWebkitTransitionEnd ||
             event_type == event_type_names::kTransitionend) {
    AddListenerType(kTransitionEndListener);
  } else if (event_type == event_type_names::kScroll) {
    AddListenerType(kScrollListener);
  } else if (event_type == event_type_names::kLoad) {
    if (Node* node = event_target.ToNode()) {
      if (IsHTMLStyleElement(*node)) {
        AddListenerType(kLoadListenerAtCapturePhaseOrAtStyleElement);
        return;
      }
    }
    if (event_target.HasCapturingEventListeners(event_type))
      AddListenerType(kLoadListenerAtCapturePhaseOrAtStyleElement);
  }
}
