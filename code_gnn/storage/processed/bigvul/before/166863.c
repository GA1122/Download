MarkingVisitor::MarkingVisitor(ThreadState* state, MarkingMode marking_mode)
    : Visitor(state),
      marking_worklist_(Heap().GetMarkingWorklist(),
                        WorklistTaskId::MainThread),
      not_fully_constructed_worklist_(Heap().GetNotFullyConstructedWorklist(),
                                      WorklistTaskId::MainThread),
      weak_callback_worklist_(Heap().GetWeakCallbackWorklist(),
                              WorklistTaskId::MainThread),
      marking_mode_(marking_mode) {
  DCHECK(state->InAtomicMarkingPause());
#if DCHECK_IS_ON()
  DCHECK(state->CheckThread());
#endif   
}
