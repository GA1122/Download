bool MarkingVisitor::RegisterWeakTable(const void* closure,
                                       EphemeronCallback iteration_callback) {
  Heap().RegisterWeakTable(const_cast<void*>(closure), iteration_callback);
  return true;
}
