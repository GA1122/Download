ListAttributeTargetObserver::ListAttributeTargetObserver(
    const AtomicString& id,
    HTMLInputElement* element)
    : IdTargetObserver(element->GetTreeScope().GetIdTargetObserverRegistry(),
                       id),
      element_(element) {}
