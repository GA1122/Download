void ThreadHeap::InvokeEphemeronCallbacks(Visitor* visitor) {
  TRACE_EVENT0("blink_gc", "ThreadHeap::InvokeEphemeronCallbacks");

  WTF::HashMap<void*, EphemeronCallback> iteration_set;
  WTF::HashMap<void*, EphemeronCallback> final_set;

  bool found_new = false;
  do {
    iteration_set = std::move(ephemeron_callbacks_);
    ephemeron_callbacks_.clear();
    for (auto& tuple : iteration_set) {
      final_set.insert(tuple.key, tuple.value);
      tuple.value(visitor, tuple.key);
    }
    found_new = !ephemeron_callbacks_.IsEmpty();
  } while (found_new);
  ephemeron_callbacks_ = std::move(final_set);
}
