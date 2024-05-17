static bool NeedsTransformForNonRootSVG(const LayoutObject& object) {
  return object.IsSVGChild() &&
         !object.LocalToSVGParentTransform().IsIdentity();
}
