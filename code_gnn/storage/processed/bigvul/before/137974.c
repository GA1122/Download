AXObject* AXLayoutObject::nextOnLine() const {
  if (!getLayoutObject())
    return nullptr;

  InlineBox* inlineBox = nullptr;
  if (getLayoutObject()->isLayoutInline())
    inlineBox = toLayoutInline(getLayoutObject())->lastLineBox();
  else if (getLayoutObject()->isText())
    inlineBox = toLayoutText(getLayoutObject())->lastTextBox();

  if (!inlineBox)
    return nullptr;

  AXObject* result = nullptr;
  for (InlineBox* next = inlineBox->nextOnLine(); next;
       next = next->nextOnLine()) {
    LayoutObject* layoutObject =
        LineLayoutAPIShim::layoutObjectFrom(next->getLineLayoutItem());
    result = axObjectCache().getOrCreate(layoutObject);
    if (result)
      break;
  }

  if (result && result->roleValue() == StaticTextRole &&
      result->children().size())
    result = result->children()[0].get();

  return result;
}
