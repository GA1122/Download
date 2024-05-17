void ListAttributeTargetObserver::Trace(blink::Visitor* visitor) {
  visitor->Trace(element_);
  IdTargetObserver::Trace(visitor);
}
