bool Extension::ProducePEM(const std::string& input, std::string* output) {
  CHECK(output);
  if (input.length() == 0)
    return false;

  return base::Base64Encode(input, output);
}
