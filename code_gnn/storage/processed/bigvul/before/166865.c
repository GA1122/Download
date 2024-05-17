void MarkingVisitor::RegisterBackingStoreReference(void* slot) {
  if (marking_mode_ != kGlobalMarkingWithCompaction)
    return;
  Heap().RegisterMovingObjectReference(
      reinterpret_cast<MovableReference*>(slot));
}
