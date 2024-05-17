const HeapVector<Member<HTMLImageElement>>& HTMLFormElement::ImageElements() {
  if (!image_elements_are_dirty_)
    return image_elements_;
  CollectImageElements(has_elements_associated_by_parser_
                           ? NodeTraversal::HighestAncestorOrSelf(*this)
                           : *this,
                       image_elements_);
  image_elements_are_dirty_ = false;
  return image_elements_;
}
