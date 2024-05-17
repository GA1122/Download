void Document::AddListenerTypeIfNeeded(const AtomicString& event_type,
                                       EventTarget& event_target) {
  if (event_type == EventTypeNames::DOMSubtreeModified) {
    UseCounter::Count(*this, WebFeature::kDOMSubtreeModifiedEvent);
    AddMutationEventListenerTypeIfEnabled(kDOMSubtreeModifiedListener);
  } else if (event_type == EventTypeNames::DOMNodeInserted) {
    UseCounter::Count(*this, WebFeature::kDOMNodeInsertedEvent);
    AddMutationEventListenerTypeIfEnabled(kDOMNodeInsertedListener);
  } else if (event_type == EventTypeNames::DOMNodeRemoved) {
    UseCounter::Count(*this, WebFeature::kDOMNodeRemovedEvent);
    AddMutationEventListenerTypeIfEnabled(kDOMNodeRemovedListener);
  } else if (event_type == EventTypeNames::DOMNodeRemovedFromDocument) {
    UseCounter::Count(*this, WebFeature::kDOMNodeRemovedFromDocumentEvent);
    AddMutationEventListenerTypeIfEnabled(kDOMNodeRemovedFromDocumentListener);
  } else if (event_type == EventTypeNames::DOMNodeInsertedIntoDocument) {
    UseCounter::Count(*this, WebFeature::kDOMNodeInsertedIntoDocumentEvent);
    AddMutationEventListenerTypeIfEnabled(kDOMNodeInsertedIntoDocumentListener);
  } else if (event_type == EventTypeNames::DOMCharacterDataModified) {
    UseCounter::Count(*this, WebFeature::kDOMCharacterDataModifiedEvent);
    AddMutationEventListenerTypeIfEnabled(kDOMCharacterDataModifiedListener);
  } else if (event_type == EventTypeNames::webkitAnimationStart ||
             event_type == EventTypeNames::animationstart) {
    AddListenerType(kAnimationStartListener);
  } else if (event_type == EventTypeNames::webkitAnimationEnd ||
             event_type == EventTypeNames::animationend) {
    AddListenerType(kAnimationEndListener);
  } else if (event_type == EventTypeNames::webkitAnimationIteration ||
             event_type == EventTypeNames::animationiteration) {
    AddListenerType(kAnimationIterationListener);
    if (View()) {
      View()->ScheduleAnimation();
    }
  } else if (event_type == EventTypeNames::webkitTransitionEnd ||
             event_type == EventTypeNames::transitionend) {
    AddListenerType(kTransitionEndListener);
  } else if (event_type == EventTypeNames::scroll) {
    AddListenerType(kScrollListener);
  } else if (event_type == EventTypeNames::load) {
    if (Node* node = event_target.ToNode()) {
      if (isHTMLStyleElement(*node)) {
        AddListenerType(kLoadListenerAtCapturePhaseOrAtStyleElement);
        return;
      }
    }
    if (event_target.HasCapturingEventListeners(event_type))
      AddListenerType(kLoadListenerAtCapturePhaseOrAtStyleElement);
  }
}
