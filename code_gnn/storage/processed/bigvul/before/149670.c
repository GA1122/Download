void ScrollAnchor::SetScroller(ScrollableArea* scroller) {
  DCHECK_NE(scroller_, scroller);
  DCHECK(scroller);
  DCHECK(scroller->IsRootFrameViewport() ||
         scroller->IsPaintLayerScrollableArea());
  scroller_ = scroller;
  ClearSelf();
}
