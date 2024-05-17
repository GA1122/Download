HttpResponseHeaders::HttpResponseHeaders(const Pickle& pickle, void** iter)
    : response_code_(-1) {
  std::string raw_input;
  if (pickle.ReadString(iter, &raw_input))
    Parse(raw_input);
}
