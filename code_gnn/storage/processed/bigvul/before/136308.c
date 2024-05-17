static bool NeedsScrollNode(const LayoutObject& object) {
  if (!object.HasOverflowClip())
    return false;
  return ToLayoutBox(object).GetScrollableArea()->ScrollsOverflow();
}
