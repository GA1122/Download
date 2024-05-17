void Document::EnqueueMediaQueryChangeListeners(
    HeapVector<Member<MediaQueryListListener>>& listeners) {
  EnsureScriptedAnimationController().EnqueueMediaQueryChangeListeners(
      listeners);
}
