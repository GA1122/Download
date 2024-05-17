bool HTMLCanvasElement::IsSupportedInteractiveCanvasFallback(
    const Element& element) {
  if (!element.IsDescendantOf(this))
    return false;


  if (IsHTMLAnchorElement(element))
    return !Traversal<HTMLImageElement>::FirstWithin(element);

  if (IsHTMLButtonElement(element))
    return true;

  if (auto* input_element = ToHTMLInputElementOrNull(element)) {
    if (input_element->type() == input_type_names::kCheckbox ||
        input_element->type() == input_type_names::kRadio ||
        input_element->IsTextButton()) {
      return true;
    }
  }

  if (auto* select_element = ToHTMLSelectElementOrNull(element)) {
    if (select_element->IsMultiple() || select_element->size() > 1)
      return true;
  }

  if (IsHTMLOptionElement(element) && element.parentNode() &&
      IsHTMLSelectElement(*element.parentNode())) {
    const HTMLSelectElement& select_element =
        ToHTMLSelectElement(*element.parentNode());
    if (select_element.IsMultiple() || select_element.size() > 1)
      return true;
  }

  if (element.FastHasAttribute(html_names::kTabindexAttr))
    return true;

  if (IsHTMLTableElement(element) ||
      element.HasTagName(html_names::kCaptionTag) ||
      element.HasTagName(html_names::kTheadTag) ||
      element.HasTagName(html_names::kTbodyTag) ||
      element.HasTagName(html_names::kTfootTag) ||
      element.HasTagName(html_names::kTrTag) ||
      element.HasTagName(html_names::kTdTag) ||
      element.HasTagName(html_names::kThTag))
    return true;

  return false;
}
