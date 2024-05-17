bool AXLayoutObject::isRichlyEditable() const {
  if (getNode() && hasRichlyEditableStyle(*getNode()))
    return true;

  if (isWebArea()) {
    Document& document = m_layoutObject->document();
    HTMLElement* body = document.body();
    if (body && hasRichlyEditableStyle(*body)) {
      AXObject* axBody = axObjectCache().getOrCreate(body);
      return axBody && axBody != axBody->ariaHiddenRoot();
    }

    return hasRichlyEditableStyle(document);
  }

  return AXNodeObject::isRichlyEditable();
}
