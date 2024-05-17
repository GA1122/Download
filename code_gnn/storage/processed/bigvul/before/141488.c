static inline const LayoutObject& ScrollbarStyleSource(
    const LayoutBox& layout_box) {
  if (layout_box.IsLayoutView()) {
    Document& doc = layout_box.GetDocument();
    if (Settings* settings = doc.GetSettings()) {
      if (!settings->GetAllowCustomScrollbarInMainFrame() &&
          layout_box.GetFrame() && layout_box.GetFrame()->IsMainFrame())
        return layout_box;
    }

    Element* body = doc.body();
    if (body && body->GetLayoutObject() && body->GetLayoutObject()->IsBox() &&
        body->GetLayoutObject()->StyleRef().HasPseudoStyle(kPseudoIdScrollbar))
      return *body->GetLayoutObject();

    Element* doc_element = doc.documentElement();
    if (doc_element && doc_element->GetLayoutObject() &&
        doc_element->GetLayoutObject()->StyleRef().HasPseudoStyle(
            kPseudoIdScrollbar))
      return *doc_element->GetLayoutObject();
  }

  return layout_box;
}
