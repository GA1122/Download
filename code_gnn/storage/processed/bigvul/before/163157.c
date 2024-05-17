bool HTMLIFrameElement::LayoutObjectIsNeeded(const ComputedStyle& style) {
  return ContentFrame() && !collapsed_by_client_ &&
         HTMLElement::LayoutObjectIsNeeded(style);
}
