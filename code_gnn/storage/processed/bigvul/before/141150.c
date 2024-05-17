HTMLLinkElement* Document::LinkCanonical() const {
  return GetLinkElement(this, [](HTMLLinkElement& link_element) {
    return link_element.RelAttribute().IsCanonical();
  });
}
