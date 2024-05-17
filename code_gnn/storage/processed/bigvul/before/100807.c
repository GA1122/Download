HttpResponseHeaders::HttpResponseHeaders(const std::string& raw_input)
    : response_code_(-1) {
  Parse(raw_input);

  UMA_HISTOGRAM_CUSTOM_ENUMERATION("Net.HttpResponseCode",
                                   MapHttpResponseCode(response_code_),
                                   GetAllHttpResponseCodes());
}
