double ThreadHeapStats::LiveObjectRateSinceLastGC() const {
  if (ObjectSizeAtLastGC() > 0)
    return static_cast<double>(MarkedObjectSize()) / ObjectSizeAtLastGC();
  return 0.0;
}
