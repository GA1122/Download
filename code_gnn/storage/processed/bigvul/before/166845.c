  static bool TraceInCollection(
      VisitorDispatcher visitor,
      blink::incremental_marking_test::StrongWeakPair& t,
      WTF::WeakHandlingFlag weakness) {
    return t.TraceInCollection(visitor, weakness);
  }
