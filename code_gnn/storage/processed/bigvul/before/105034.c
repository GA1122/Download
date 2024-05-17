void FakeURLFetcherFactory::SetFakeResponse(const std::string& url,
                                            const std::string& response_data,
                                            bool success) {
  fake_responses_[GURL(url)] = std::make_pair(response_data, success);
}
