void HTMLFormElement::AddToPastNamesMap(Element* element,
                                        const AtomicString& past_name) {
  if (past_name.IsEmpty())
    return;
  if (!past_names_map_)
    past_names_map_ = new PastNamesMap;
  past_names_map_->Set(past_name, element);
}
