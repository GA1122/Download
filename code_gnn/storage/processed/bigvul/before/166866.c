void MarkingVisitor::RegisterWeakCallback(void* object, WeakCallback callback) {
  if (marking_mode_ == kSnapshotMarking)
    return;
  weak_callback_worklist_.Push({object, callback});
}
