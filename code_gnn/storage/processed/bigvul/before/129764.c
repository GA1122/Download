  static void Delete(base::ConditionVariable* instance) {
    instance->~ConditionVariable();
  }
