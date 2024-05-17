  void CheckFails(
      const WebURLRequest& request,
      WebAssociatedURLLoaderOptions options = WebAssociatedURLLoaderOptions()) {
    expected_loader_ = CreateAssociatedURLLoader(options);
    EXPECT_TRUE(expected_loader_);
    did_fail_ = false;
    expected_loader_->LoadAsynchronously(request, this);
    EXPECT_FALSE(did_fail_);
    RunPendingTasks();
    EXPECT_TRUE(did_fail_);
    EXPECT_FALSE(did_receive_response_);
  }
