void Document::AddComputedStyleMapItem(
    Element* element,
    StylePropertyMapReadOnly* computed_style) {
  element_computed_style_map_.insert(element, computed_style);
}
