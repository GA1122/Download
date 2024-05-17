void ThreadHeap::ProcessMarkingStack(Visitor* visitor) {
  bool complete = AdvanceMarkingStackProcessing(
      visitor, std::numeric_limits<double>::infinity());
  CHECK(complete);
}
