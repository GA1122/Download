void HTMLFormElement::Associate(HTMLImageElement& e) {
  image_elements_are_dirty_ = true;
  image_elements_.clear();
}
