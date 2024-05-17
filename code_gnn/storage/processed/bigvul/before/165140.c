void HTMLFormElement::GetNamedElements(
    const AtomicString& name,
    HeapVector<Member<Element>>& named_items) {
  elements()->NamedItems(name, named_items);

  Element* element_from_past = ElementFromPastNamesMap(name);
  if (named_items.size() && named_items.front() != element_from_past) {
    AddToPastNamesMap(named_items.front().Get(), name);
  } else if (element_from_past && named_items.IsEmpty()) {
    named_items.push_back(element_from_past);
    UseCounter::Count(GetDocument(),
                      WebFeature::kFormNameAccessForPastNamesMap);
  }
}
