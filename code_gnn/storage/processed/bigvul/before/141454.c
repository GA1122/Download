PaintLayerScrollableArea::DelayScrollOffsetClampScope::NeedsClampList() {
  DEFINE_STATIC_LOCAL(
      Persistent<HeapVector<Member<PaintLayerScrollableArea>>>,
      needs_clamp_list,
      (MakeGarbageCollected<HeapVector<Member<PaintLayerScrollableArea>>>()));
  return *needs_clamp_list;
}
