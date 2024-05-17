void Document::ActiveChainNodeDetached(Element& element) {
  if (element == active_element_)
    active_element_ = SkipDisplayNoneAncestors(&element);
}
