  static bool IsEmptyValue(
      const blink::incremental_marking_test::StrongWeakPair& value) {
    return !value.first;
  }
