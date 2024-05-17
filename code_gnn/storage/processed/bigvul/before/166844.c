  bool TraceInCollection(VisitorDispatcher visitor,
                         WTF::WeakHandlingFlag weakness) {
    visitor->Trace(first);
    if (weakness == WTF::kNoWeakHandling) {
      visitor->Trace(second);
    }
    return false;
  }
