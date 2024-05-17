  void OnResult(std::unique_ptr<runtime::EvaluateResult> result) {
    EXPECT_TRUE(result->GetResult()->HasValue());
    EXPECT_EQ("[\"en-UK\",\"DE\",\"FR\"]",
              result->GetResult()->GetValue()->GetString());
    FinishAsynchronousTest();
  }
