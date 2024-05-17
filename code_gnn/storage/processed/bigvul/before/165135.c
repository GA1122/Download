void HTMLFormElement::Disassociate(HTMLImageElement& e) {
  image_elements_are_dirty_ = true;
  image_elements_.clear();
  RemoveFromPastNamesMap(e);
}
