void FetchManager::Trace(blink::Visitor* visitor) {
  visitor->Trace(loaders_);
  ContextLifecycleObserver::Trace(visitor);
}
