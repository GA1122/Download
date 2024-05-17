  static void ConstructDeletedValue(
      blink::incremental_marking_test::StrongWeakPair& slot,
      bool) {
    new (NotNull, &slot)
        blink::incremental_marking_test::StrongWeakPair(kHashTableDeletedValue);
  }
