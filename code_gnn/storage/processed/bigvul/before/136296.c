static bool NeedsClipPathClip(const LayoutObject& object) {
  if (!object.StyleRef().ClipPath())
    return false;

  return object.FirstFragment().ClipPathPath();
}
