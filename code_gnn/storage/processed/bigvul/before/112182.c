StringValue* MakeEnumValue(T t, const char* (*converter_fn)(T)) {
  return Value::CreateStringValue(converter_fn(t));
}
