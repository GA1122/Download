  explicit FromV8ValueState(bool avoid_identity_hash_for_testing)
      : max_recursion_depth_(kMaxRecursionDepth),
        avoid_identity_hash_for_testing_(avoid_identity_hash_for_testing) {}
