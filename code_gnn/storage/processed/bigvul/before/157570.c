  void OnFirstResult(std::unique_ptr<runtime::EvaluateResult> result) {
    EXPECT_TRUE(result->GetResult()->HasValue());
    EXPECT_EQ(24 * 7, result->GetResult()->GetValue()->GetInt());

    HeadlessDevToolsTarget* devtools_target =
        web_contents_->GetDevToolsTarget();

    EXPECT_TRUE(devtools_target->IsAttached());
    devtools_target->DetachClient(other_devtools_client_.get());
    EXPECT_FALSE(devtools_target->IsAttached());
    devtools_target->AttachClient(devtools_client_.get());
    EXPECT_TRUE(devtools_target->IsAttached());

    devtools_client_->GetRuntime()->Evaluate(
        "27 * 4",
        base::BindOnce(&HeadlessDevToolsClientAttachTest::OnSecondResult,
                       base::Unretained(this)));
  }
