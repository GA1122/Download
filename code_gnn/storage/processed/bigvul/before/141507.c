void PaintLayerScrollableArea::ScrollbarManager::Trace(
    blink::Visitor* visitor) {
  visitor->Trace(scrollable_area_);
  visitor->Trace(h_bar_);
  visitor->Trace(v_bar_);
}
