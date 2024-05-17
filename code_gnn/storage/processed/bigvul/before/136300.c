static bool NeedsInnerBorderRadiusClip(const LayoutObject& object) {
  if (!object.StyleRef().HasBorderRadius())
    return false;
  if (object.IsBox() && NeedsOverflowClip(object))
    return true;
  if (object.IsLayoutReplaced() && !object.IsSVGRoot())
    return true;
  return false;
}
