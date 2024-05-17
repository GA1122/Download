bool TestURLFetcher::GetResponseAsString(
    std::string* out_response_string) const {
  if (GetResponseDestinationForTesting() != STRING)
    return false;

  *out_response_string = fake_response_string_;
  return true;
}
