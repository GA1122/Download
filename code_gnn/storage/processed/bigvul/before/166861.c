std::unique_ptr<MarkingVisitor> MarkingVisitor::Create(ThreadState* state,
                                                       MarkingMode mode) {
  return std::make_unique<MarkingVisitor>(state, mode);
}
