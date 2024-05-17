StylePropertyMapReadOnly* Document::RemoveComputedStyleMapItem(
    Element* element) {
  StylePropertyMapReadOnly* computed_style =
      element_computed_style_map_.at(element);
  element_computed_style_map_.erase(element);
  return computed_style;
}
