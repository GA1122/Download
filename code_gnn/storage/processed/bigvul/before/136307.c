static bool NeedsSVGLocalToBorderBoxTransform(const LayoutObject& object) {
  return object.IsSVGRoot();
}
