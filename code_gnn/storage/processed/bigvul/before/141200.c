void Document::RemoveFromTopLayer(Element* element) {
  if (!element->IsInTopLayer())
    return;
  wtf_size_t position = top_layer_elements_.Find(element);
  DCHECK_NE(position, kNotFound);
  top_layer_elements_.EraseAt(position);
  element->SetIsInTopLayer(false);
}
