void FragmentPaintPropertyTreeBuilder::UpdateClipPathCache() {
  if (fragment_data_.IsClipPathCacheValid())
    return;

  if (!object_.StyleRef().ClipPath())
    return;

  base::Optional<FloatRect> bounding_box =
      ClipPathClipper::LocalClipPathBoundingBox(object_);
  if (!bounding_box) {
    fragment_data_.SetClipPathCache(base::nullopt, nullptr);
    return;
  }
  bounding_box->MoveBy(FloatPoint(fragment_data_.PaintOffset()));

  bool is_valid = false;
  base::Optional<Path> path = ClipPathClipper::PathBasedClip(
      object_, object_.IsSVGChild(),
      ClipPathClipper::LocalReferenceBox(object_), is_valid);
  DCHECK(is_valid);
  if (path)
    path->Translate(ToFloatSize(FloatPoint(fragment_data_.PaintOffset())));
  fragment_data_.SetClipPathCache(
      EnclosingIntRect(*bounding_box),
      path ? AdoptRef(new RefCountedPath(std::move(*path))) : nullptr);
}
