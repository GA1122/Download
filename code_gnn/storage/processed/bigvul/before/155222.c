void HTMLFormElement::getNamedElements(
    const AtomicString& name,
    HeapVector<Member<Element>>& namedItems) {
  elements()->namedItems(name, namedItems);

  Element* elementFromPast = elementFromPastNamesMap(name);
  if (namedItems.size() && namedItems.first() != elementFromPast) {
    addToPastNamesMap(namedItems.first().get(), name);
  } else if (elementFromPast && namedItems.isEmpty()) {
    namedItems.append(elementFromPast);
    UseCounter::count(document(), UseCounter::FormNameAccessForPastNamesMap);
  }
}
