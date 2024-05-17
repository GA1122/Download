void Document::ViewportDefiningElementDidChange() {
  HTMLBodyElement* body = FirstBodyElement();
  if (!body)
    return;
  if (body->NeedsReattachLayoutTree())
    return;
  LayoutObject* layout_object = body->GetLayoutObject();
  if (layout_object && layout_object->IsLayoutBlock()) {
    layout_object->SetStyle(ComputedStyle::Clone(*layout_object->Style()));
    if (layout_object->HasLayer()) {
      ToLayoutBoxModelObject(layout_object)
          ->Layer()
          ->SetNeedsCompositingReasonsUpdate();
    }
  }
}
