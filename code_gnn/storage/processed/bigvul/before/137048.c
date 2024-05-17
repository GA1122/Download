InputType* InputType::Create(HTMLInputElement& element,
                             const AtomicString& type_name) {
  InputTypeFactoryFunction factory =
      type_name.IsEmpty() ? nullptr : FactoryMap()->at(type_name);
  if (!factory)
    factory = TextInputType::Create;
  return factory(element);
}
