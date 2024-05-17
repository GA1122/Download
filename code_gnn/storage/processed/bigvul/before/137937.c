String AXLayoutObject::fontFamily() const {
  if (!getLayoutObject())
    return AXNodeObject::fontFamily();

  const ComputedStyle* style = getLayoutObject()->style();
  if (!style)
    return AXNodeObject::fontFamily();

  FontDescription& fontDescription =
      const_cast<FontDescription&>(style->getFontDescription());
  return fontDescription.firstFamily().family();
}
