void HTMLFormElement::AnonymousNamedGetter(
    const AtomicString& name,
    RadioNodeListOrElement& return_value) {
  {
    HeapVector<Member<Element>> elements;
    GetNamedElements(name, elements);
    if (elements.IsEmpty())
      return;
  }

  HeapVector<Member<Element>> elements;
  GetNamedElements(name, elements);
  DCHECK(!elements.IsEmpty());

  bool only_match_img =
      !elements.IsEmpty() && IsHTMLImageElement(*elements.front());
  if (only_match_img) {
    UseCounter::Count(GetDocument(),
                      WebFeature::kFormNameAccessForImageElement);
    for (auto& element : elements) {
      if (IsHTMLImageElement(*element) && !element->IsDescendantOf(this)) {
        UseCounter::Count(
            GetDocument(),
            WebFeature::kFormNameAccessForNonDescendantImageElement);
        break;
      }
    }
  }
  if (elements.size() == 1) {
    return_value.SetElement(elements.at(0));
    return;
  }

  return_value.SetRadioNodeList(GetRadioNodeList(name, only_match_img));
}
