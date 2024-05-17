ScopedVector<GestureEvent>* GestureProviderAura::GetAndResetPendingGestures() {
  if (pending_gestures_.empty())
    return NULL;
  ScopedVector<GestureEvent>* old_pending_gestures =
      new ScopedVector<GestureEvent>();
  old_pending_gestures->swap(pending_gestures_);
  return old_pending_gestures;
}
