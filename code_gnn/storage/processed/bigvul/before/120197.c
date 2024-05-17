Region Layer::VisibleContentOpaqueRegion() const {
  if (contents_opaque())
    return visible_content_rect();
  return Region();
}
