void ThreadHeap::RegisterMovingObjectCallback(MovableReference reference,
                                              MovingObjectCallback callback,
                                              void* callback_data) {
  DCHECK(reference);
  Compaction()->RegisterMovingObjectCallback(reference, callback,
                                             callback_data);
}
