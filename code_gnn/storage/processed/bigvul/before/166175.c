  void GenerateStreamAndWaitForFailure(
      int page_request_id,
      const StreamControls& controls,
      MediaStreamRequestResult expected_result) {
    base::RunLoop run_loop;
    EXPECT_CALL(*host_,
                OnStreamGenerationFailure(page_request_id, expected_result));
    host_->OnGenerateStream(page_request_id, controls, run_loop.QuitClosure());
    run_loop.Run();
  }
