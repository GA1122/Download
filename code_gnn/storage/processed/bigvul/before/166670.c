bool ThreadHeap::AdvanceLazySweep(double deadline_seconds) {
  for (int i = 0; i < BlinkGC::kNumberOfArenas; i++) {
    double slack = 0.001;
    double remaining_budget =
        deadline_seconds - slack - CurrentTimeTicksInSeconds();
    if (remaining_budget <= 0 ||
        !arenas_[i]->LazySweepWithDeadline(deadline_seconds)) {
      return false;
    }
  }
  return true;
}
