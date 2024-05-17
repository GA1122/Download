StylePropertyMapReadOnly* Document::ComputedStyleMap(Element* element) {
  ElementComputedStyleMap::AddResult add_result =
      element_computed_style_map_.insert(element, nullptr);
  if (add_result.is_new_entry)
    add_result.stored_value->value = ComputedStylePropertyMap::Create(element);
  return add_result.stored_value->value;
}
