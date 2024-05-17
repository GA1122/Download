Element* Document::ViewportDefiningElement() const {
  Element* root_element = documentElement();
  Element* body_element = body();
  if (!root_element)
    return nullptr;
  const ComputedStyle* root_style = root_element->GetComputedStyle();
  if (!root_style)
    return nullptr;
  if (body_element && root_style->IsOverflowVisible() &&
      IsHTMLHtmlElement(*root_element))
    return body_element;
  return root_element;
}
