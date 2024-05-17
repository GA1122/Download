void ThreadHeap::CommitCallbackStacks() {
  marking_worklist_.reset(new MarkingWorklist());
  not_fully_constructed_worklist_.reset(new NotFullyConstructedWorklist());
  weak_callback_worklist_.reset(new WeakCallbackWorklist());
  DCHECK(ephemeron_callbacks_.IsEmpty());
}
