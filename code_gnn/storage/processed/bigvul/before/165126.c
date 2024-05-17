void HTMLFormElement::Associate(ListedElement& e) {
  listed_elements_are_dirty_ = true;
  listed_elements_.clear();
  if (ToHTMLElement(e).FastHasAttribute(formAttr))
    has_elements_associated_by_form_attribute_ = true;
}
