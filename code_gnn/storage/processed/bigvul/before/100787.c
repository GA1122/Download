std::vector<int> GetAllHttpResponseCodes() {
  std::vector<int> codes;
  codes.reserve(
      HISTOGRAM_MAX_HTTP_RESPONSE_CODE - HISTOGRAM_MIN_HTTP_RESPONSE_CODE + 2);
  codes.push_back(0);
  for (int i = HISTOGRAM_MIN_HTTP_RESPONSE_CODE;
       i <= HISTOGRAM_MAX_HTTP_RESPONSE_CODE; ++i)
    codes.push_back(i);
  return codes;
}
