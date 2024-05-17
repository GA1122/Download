int8_t ClampMin(int8_t value) {
  const static int8_t kMinInt8Value = INT8_MIN + 1;
  return value < kMinInt8Value ? kMinInt8Value : value;
}
