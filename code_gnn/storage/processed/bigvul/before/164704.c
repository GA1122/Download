  void IsolateOrigin(const std::string& hostname) {
    IsolateOriginsForTesting(embedded_test_server(), shell()->web_contents(),
                             {hostname});
  }
