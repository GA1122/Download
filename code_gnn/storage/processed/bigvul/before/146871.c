void Document::ViewportDefiningElementDidChange() {
  HTMLBodyElement* body = FirstBodyElement();
  if (!body)
    return;
  LayoutObject* layout_object = body->GetLayoutObject();
  if (layout_object && layout_object->IsLayoutBlock()) {
    layout_object->SetStyle(ComputedStyle::Clone(*layout_object->Style()));
  }
}
