void ThreadHeap::RegisterMovingObjectReference(MovableReference* slot) {
  DCHECK(slot);
  DCHECK(*slot);
  Compaction()->RegisterMovingObjectReference(slot);
}
