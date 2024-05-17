void Document::addListenerTypeIfNeeded(const AtomicString& eventType)
{
    if (eventType == EventTypeNames::DOMSubtreeModified) {
        UseCounter::count(*this, UseCounter::DOMSubtreeModifiedEvent);
        addMutationEventListenerTypeIfEnabled(DOMSUBTREEMODIFIED_LISTENER);
    } else if (eventType == EventTypeNames::DOMNodeInserted) {
        UseCounter::count(*this, UseCounter::DOMNodeInsertedEvent);
        addMutationEventListenerTypeIfEnabled(DOMNODEINSERTED_LISTENER);
    } else if (eventType == EventTypeNames::DOMNodeRemoved) {
        UseCounter::count(*this, UseCounter::DOMNodeRemovedEvent);
        addMutationEventListenerTypeIfEnabled(DOMNODEREMOVED_LISTENER);
    } else if (eventType == EventTypeNames::DOMNodeRemovedFromDocument) {
        UseCounter::count(*this, UseCounter::DOMNodeRemovedFromDocumentEvent);
        addMutationEventListenerTypeIfEnabled(DOMNODEREMOVEDFROMDOCUMENT_LISTENER);
    } else if (eventType == EventTypeNames::DOMNodeInsertedIntoDocument) {
        UseCounter::count(*this, UseCounter::DOMNodeInsertedIntoDocumentEvent);
        addMutationEventListenerTypeIfEnabled(DOMNODEINSERTEDINTODOCUMENT_LISTENER);
    } else if (eventType == EventTypeNames::DOMCharacterDataModified) {
        UseCounter::count(*this, UseCounter::DOMCharacterDataModifiedEvent);
        addMutationEventListenerTypeIfEnabled(DOMCHARACTERDATAMODIFIED_LISTENER);
    } else if (eventType == EventTypeNames::webkitAnimationStart || eventType == EventTypeNames::animationstart) {
        addListenerType(ANIMATIONSTART_LISTENER);
    } else if (eventType == EventTypeNames::webkitAnimationEnd || eventType == EventTypeNames::animationend) {
        addListenerType(ANIMATIONEND_LISTENER);
    } else if (eventType == EventTypeNames::webkitAnimationIteration || eventType == EventTypeNames::animationiteration) {
        addListenerType(ANIMATIONITERATION_LISTENER);
        if (view()) {
            view()->scheduleAnimation();
        }
    } else if (eventType == EventTypeNames::webkitTransitionEnd || eventType == EventTypeNames::transitionend) {
        addListenerType(TRANSITIONEND_LISTENER);
    } else if (eventType == EventTypeNames::scroll) {
        addListenerType(SCROLL_LISTENER);
    }
}
