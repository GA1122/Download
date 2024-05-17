  void Respond(const WebURLResponse& response) {
    EXPECT_TRUE(active_loader());
    data_provider()->DidReceiveResponse(response);
    base::RunLoop().RunUntilIdle();
  }
