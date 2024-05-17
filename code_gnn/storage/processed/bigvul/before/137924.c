RGBA32 AXLayoutObject::computeBackgroundColor() const {
  if (!getLayoutObject())
    return AXNodeObject::backgroundColor();

  Color blendedColor = Color::transparent;
  for (LayoutObject* layoutObject = getLayoutObject(); layoutObject;
       layoutObject = layoutObject->parent()) {
    const AXObject* axParent = axObjectCache().getOrCreate(layoutObject);
    if (axParent && axParent != this) {
      Color parentColor = axParent->backgroundColor();
      blendedColor = parentColor.blend(blendedColor);
      return blendedColor.rgb();
    }

    const ComputedStyle* style = layoutObject->style();
    if (!style || !style->hasBackground())
      continue;

    Color currentColor =
        style->visitedDependentColor(CSSPropertyBackgroundColor);
    blendedColor = currentColor.blend(blendedColor);
    if (!blendedColor.hasAlpha())
      break;
  }

  if (blendedColor.hasAlpha()) {
    FrameView* view = documentFrameView();
    if (view) {
      Color documentBaseColor = view->baseBackgroundColor();
      blendedColor = documentBaseColor.blend(blendedColor);
    } else {
      blendedColor.blendWithWhite();
    }
  }

  return blendedColor.rgb();
}
