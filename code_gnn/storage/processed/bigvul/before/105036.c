void TestURLFetcher::SetResponseString(const std::string& response) {
  SetResponseDestinationForTesting(STRING);
  fake_response_string_ = response;
}
