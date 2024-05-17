int16_t ClampMin(int16_t value) {
  const static int16_t kMinInt16Value = INT16_MIN + 1;
  return value < kMinInt16Value ? kMinInt16Value : value;
}
