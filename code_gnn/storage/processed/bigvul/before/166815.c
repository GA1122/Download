  static bool IsDeletedValue(
      const blink::incremental_marking_test::StrongWeakPair& value) {
    return value.IsHashTableDeletedValue();
  }
