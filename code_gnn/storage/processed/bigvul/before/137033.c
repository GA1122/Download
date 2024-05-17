String HTMLInputElement::value() const {
  switch (input_type_->GetValueMode()) {
    case ValueMode::kFilename:
      return input_type_->ValueInFilenameValueMode();
    case ValueMode::kDefault:
      return FastGetAttribute(valueAttr);
    case ValueMode::kDefaultOn: {
      AtomicString value_string = FastGetAttribute(valueAttr);
      return value_string.IsNull() ? "on" : value_string;
    }
    case ValueMode::kValue:
      return non_attribute_value_;
  }
  NOTREACHED();
  return g_empty_string;
}
