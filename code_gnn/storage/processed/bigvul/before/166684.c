double ThreadHeapStats::EstimatedMarkingTime() {
  if (estimated_marking_time_per_byte_ == 0)
    return 0.008;

  return estimated_marking_time_per_byte_ *
         (AllocatedObjectSize() + MarkedObjectSize());
}
