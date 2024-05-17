bool AXLayoutObject::isEditable() const {
  if (getLayoutObject() && getLayoutObject()->isTextControl())
    return true;

  if (getNode() && hasEditableStyle(*getNode()))
    return true;

  if (isWebArea()) {
    Document& document = getLayoutObject()->document();
    HTMLElement* body = document.body();
    if (body && hasEditableStyle(*body)) {
      AXObject* axBody = axObjectCache().getOrCreate(body);
      return axBody && axBody != axBody->ariaHiddenRoot();
    }

    return hasEditableStyle(document);
  }

  return AXNodeObject::isEditable();
}
