  virtual void Trace(blink::Visitor* visitor) {
    visitor->Trace(next_);
    visitor->Trace(obj_);
  }
