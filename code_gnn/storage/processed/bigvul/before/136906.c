bool HTMLInputElement::HasValidDataListOptions() const {
  HTMLDataListElement* data_list = DataList();
  if (!data_list)
    return false;
  HTMLDataListOptionsCollection* options = data_list->options();
  for (unsigned i = 0; HTMLOptionElement* option = options->Item(i); ++i) {
    if (!option->value().IsEmpty() && !option->IsDisabledFormControl() &&
        IsValidValue(option->value()))
      return true;
  }
  return false;
}
