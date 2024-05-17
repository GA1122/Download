void SVGElement::RemovedEventListener(
    const AtomicString& event_type,
    const RegisteredEventListener& registered_listener) {
  Node::RemovedEventListener(event_type, registered_listener);

  HeapHashSet<WeakMember<SVGElement>> instances;
  CollectInstancesForSVGElement(this, instances);
  EventListenerOptions* options = registered_listener.Options();
  const EventListener* listener = registered_listener.Callback();
  for (SVGElement* shadow_tree_element : instances) {
    DCHECK(shadow_tree_element);

    shadow_tree_element->Node::RemoveEventListenerInternal(event_type, listener,
                                                           options);
  }
}
