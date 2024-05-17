void HTMLCanvasElement::Trace(Visitor* visitor) {
  visitor->Trace(listeners_);
  visitor->Trace(context_);
  ContextLifecycleObserver::Trace(visitor);
  PageVisibilityObserver::Trace(visitor);
  HTMLElement::Trace(visitor);
}
