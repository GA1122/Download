void HTMLFormElement::CollectListedElements(
    Node& root,
    ListedElement::List& elements) const {
  elements.clear();
  for (HTMLElement& element : Traversal<HTMLElement>::StartsAfter(root)) {
    ListedElement* listed_element = nullptr;
    if (element.IsFormControlElement())
      listed_element = ToHTMLFormControlElement(&element);
    else if (auto* object = ToHTMLObjectElementOrNull(element))
      listed_element = object;
    else
      continue;
    if (listed_element->Form() == this)
      elements.push_back(listed_element);
  }
}
