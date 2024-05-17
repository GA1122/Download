  void Trace(Visitor* visitor) {
    visitor->Trace(obj_);
    visitor->Trace(prev_);
    visitor->Trace(next_);
  }
