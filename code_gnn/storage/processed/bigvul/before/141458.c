PaintLayerScrollableArea::ScrollingBackgroundDisplayItemClient::OwnerNodeId()
    const {
  return static_cast<const DisplayItemClient*>(scrollable_area_->GetLayoutBox())
      ->OwnerNodeId();
}
