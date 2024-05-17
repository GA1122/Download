LayoutBox* PaintLayerScrollableArea::GetLayoutBox() const {
  return layer_ ? layer_->GetLayoutBox() : nullptr;
}
