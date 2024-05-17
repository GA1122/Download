bool IsBytes(DataElement::Type type) {
  return type == DataElement::TYPE_BYTES ||
         type == DataElement::TYPE_BYTES_DESCRIPTION;
}
