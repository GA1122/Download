  void FinishLoading() {
    EXPECT_TRUE(active_loader());
    data_provider()->DidFinishLoading();
    base::RunLoop().RunUntilIdle();
  }
