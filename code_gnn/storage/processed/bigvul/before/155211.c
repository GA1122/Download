void HTMLFormElement::collectAssociatedElements(
    Node& root,
    FormAssociatedElement::List& elements) const {
  elements.clear();
  for (HTMLElement& element : Traversal<HTMLElement>::startsAfter(root)) {
    FormAssociatedElement* associatedElement = 0;
    if (element.isFormControlElement())
      associatedElement = toHTMLFormControlElement(&element);
    else if (isHTMLObjectElement(element))
      associatedElement = toHTMLObjectElement(&element);
    else
      continue;
    if (associatedElement->form() == this)
      elements.append(associatedElement);
  }
}
