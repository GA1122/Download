StringValue* MakeInt64Value(int64 x) {
  return Value::CreateStringValue(base::Int64ToString(x));
}
