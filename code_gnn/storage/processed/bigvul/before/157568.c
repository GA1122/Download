  void OnFirstResult(std::unique_ptr<runtime::EvaluateResult> result) {
    EXPECT_TRUE(result->GetResult()->HasValue());
    EXPECT_EQ(3, result->GetResult()->GetValue()->GetInt());
  }
