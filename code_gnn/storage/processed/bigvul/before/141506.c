void PaintLayerScrollableArea::Trace(blink::Visitor* visitor) {
  visitor->Trace(scrollbar_manager_);
  visitor->Trace(scroll_anchor_);
  visitor->Trace(scrolling_background_display_item_client_);
  ScrollableArea::Trace(visitor);
}
