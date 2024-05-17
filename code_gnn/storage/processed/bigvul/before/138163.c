int AXObject::indexInParent() const {
  if (!parentObject())
    return 0;

  const auto& siblings = parentObject()->children();
  int childCount = siblings.size();

  for (int index = 0; index < childCount; ++index) {
    if (siblings[index].get() == this) {
      return index;
    }
  }
  return 0;
}
