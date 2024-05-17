bool InputType::IsFormDataAppendable() const {
  return !GetElement().GetName().IsEmpty();
}
