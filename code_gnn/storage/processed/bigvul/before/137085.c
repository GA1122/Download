const AtomicString& InputType::NormalizeTypeName(
    const AtomicString& type_name) {
  if (type_name.IsEmpty())
    return InputTypeNames::text;
  InputTypeFactoryMap::const_iterator it =
      FactoryMap()->find(type_name.LowerASCII());
  return it == FactoryMap()->end() ? InputTypeNames::text : it->key;
}
