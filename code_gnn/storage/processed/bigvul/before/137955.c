static bool isLinkable(const AXObject& object) {
  if (!object.getLayoutObject())
    return false;

  return object.isLink() || object.isImage() ||
         object.getLayoutObject()->isText();
}
