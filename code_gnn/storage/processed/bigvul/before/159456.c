int32_t ClampMin(int32_t value) {
  const static int32_t kMinInt32Value = INT32_MIN + 1;
  return value < kMinInt32Value ? kMinInt32Value : value;
}
