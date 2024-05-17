AXObject* AXLayoutObject::previousOnLine() const {
  if (!getLayoutObject())
    return nullptr;

  InlineBox* inlineBox = nullptr;
  if (getLayoutObject()->isLayoutInline())
    inlineBox = toLayoutInline(getLayoutObject())->firstLineBox();
  else if (getLayoutObject()->isText())
    inlineBox = toLayoutText(getLayoutObject())->firstTextBox();

  if (!inlineBox)
    return nullptr;

  AXObject* result = nullptr;
  for (InlineBox* prev = inlineBox->prevOnLine(); prev;
       prev = prev->prevOnLine()) {
    LayoutObject* layoutObject =
        LineLayoutAPIShim::layoutObjectFrom(prev->getLineLayoutItem());
    result = axObjectCache().getOrCreate(layoutObject);
    if (result)
      break;
  }

  if (result && result->roleValue() == StaticTextRole &&
      result->children().size())
    result = result->children()[result->children().size() - 1].get();

  return result;
}
