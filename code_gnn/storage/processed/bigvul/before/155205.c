void HTMLFormElement::anonymousNamedGetter(
    const AtomicString& name,
    RadioNodeListOrElement& returnValue) {
  {
    HeapVector<Member<Element>> elements;
    getNamedElements(name, elements);
    if (elements.isEmpty())
      return;
  }

  HeapVector<Member<Element>> elements;
  getNamedElements(name, elements);
  DCHECK(!elements.isEmpty());

  bool onlyMatchImg =
      !elements.isEmpty() && isHTMLImageElement(*elements.first());
  if (onlyMatchImg) {
    UseCounter::count(document(), UseCounter::FormNameAccessForImageElement);
    for (auto& element : elements) {
      if (isHTMLImageElement(*element) && !element->isDescendantOf(this)) {
        UseCounter::count(
            document(), UseCounter::FormNameAccessForNonDescendantImageElement);
        break;
      }
    }
  }
  if (elements.size() == 1) {
    returnValue.setElement(elements.at(0));
    return;
  }

  returnValue.setRadioNodeList(radioNodeList(name, onlyMatchImg));
}
