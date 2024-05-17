void PaintLayerScrollableArea::ClampScrollOffsetAfterOverflowChange() {
  if (HasBeenDisposed())
    return;


  if (DelayScrollOffsetClampScope::ClampingIsDelayed()) {
    DelayScrollOffsetClampScope::SetNeedsClamp(this);
    return;
  }

  UpdateScrollDimensions();
  if (ScrollOriginChanged())
    SetScrollOffsetUnconditionally(ClampScrollOffset(GetScrollOffset()));
  else
    ScrollableArea::SetScrollOffset(GetScrollOffset(), kClampingScroll);

  SetNeedsScrollOffsetClamp(false);
  ResetScrollOriginChanged();
  scrollbar_manager_.DestroyDetachedScrollbars();
}
