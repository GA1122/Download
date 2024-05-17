const AtomicString& HTMLInputElement::GetName() const {
  return name_.IsNull() ? g_empty_atom : name_;
}
