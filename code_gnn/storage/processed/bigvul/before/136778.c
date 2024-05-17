  virtual void Trace(blink::Visitor* visitor) {
    visitor->Trace(event_);
    visitor->Trace(window_);
    PausableTimer::Trace(visitor);
  }
