void ScrollAnchor::Clear() {
  LayoutObject* layout_object =
      anchor_object_ ? anchor_object_ : ScrollerLayoutBox(scroller_);
  PaintLayer* layer = nullptr;
  if (LayoutObject* parent = layout_object->Parent())
    layer = parent->EnclosingLayer();

  while (layer) {
    if (PaintLayerScrollableArea* scrollable_area =
            layer->GetScrollableArea()) {
      ScrollAnchor* anchor = scrollable_area->GetScrollAnchor();
      DCHECK(anchor);
      anchor->ClearSelf();
    }
    layer = layer->Parent();
  }
}
