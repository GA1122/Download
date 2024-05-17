bool PaintLayerScrollableArea::HitTestResizerInFragments(
    const PaintLayerFragments& layer_fragments,
    const HitTestLocation& hit_test_location) const {
  if (!GetLayoutBox()->CanResize())
    return false;

  if (layer_fragments.IsEmpty())
    return false;

  for (int i = layer_fragments.size() - 1; i >= 0; --i) {
    const PaintLayerFragment& fragment = layer_fragments.at(i);
    if (fragment.background_rect.Intersects(hit_test_location) &&
        ResizerCornerRect(PixelSnappedIntRect(fragment.layer_bounds),
                          kResizerForPointer)
            .Contains(hit_test_location.RoundedPoint()))
      return true;
  }

  return false;
}
