void HTMLFormElement::CopyNonAttributePropertiesFromElement(
    const Element& source) {
  was_demoted_ = static_cast<const HTMLFormElement&>(source).was_demoted_;
  HTMLElement::CopyNonAttributePropertiesFromElement(source);
}
