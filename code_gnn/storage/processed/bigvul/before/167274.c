    void Trace(blink::Visitor* visitor) {
      visitor->Trace(updater_);
      visitor->Trace(response_);
      visitor->Trace(loader_);
    }
