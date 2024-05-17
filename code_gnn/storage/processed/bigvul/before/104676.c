  void RunTestcases(const Testcase* testcases, size_t num_testcases) {
    for (size_t i = 0; i < num_testcases; ++i) {
      LoadAndExpectError(testcases[i].manifest, testcases[i].expected_error);
    }
  }
