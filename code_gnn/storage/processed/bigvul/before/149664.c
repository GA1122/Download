bool ScrollAnchor::RefersTo(const LayoutObject* layout_object) const {
  return anchor_object_ == layout_object;
}
