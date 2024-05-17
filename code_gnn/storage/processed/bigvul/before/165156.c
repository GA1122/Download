void HTMLFormElement::RemoveFromPastNamesMap(HTMLElement& element) {
  if (!past_names_map_)
    return;
  for (auto& it : *past_names_map_) {
    if (it.value == &element) {
      it.value = nullptr;
    }
  }
}
