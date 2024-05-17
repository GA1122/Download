HTMLInputElement::FilteredDataListOptions() const {
  HeapVector<Member<HTMLOptionElement>> filtered;
  HTMLDataListElement* data_list = DataList();
  if (!data_list)
    return filtered;

  String value = InnerEditorValue();
  if (Multiple() && type() == InputTypeNames::email) {
    Vector<String> emails;
    value.Split(',', true, emails);
    if (!emails.IsEmpty())
      value = emails.back().StripWhiteSpace();
  }

  HTMLDataListOptionsCollection* options = data_list->options();
  filtered.ReserveCapacity(options->length());
  value = value.FoldCase();
  for (unsigned i = 0; i < options->length(); ++i) {
    HTMLOptionElement* option = options->Item(i);
    DCHECK(option);
    if (!value.IsEmpty()) {
      if (option->value().FoldCase().Find(value) == kNotFound &&
          option->label().FoldCase().Find(value) == kNotFound)
        continue;
    }
    if (option->value().IsEmpty() || option->IsDisabledFormControl() ||
        !IsValidValue(option->value()))
      continue;
    filtered.push_back(option);
  }
  return filtered;
}
