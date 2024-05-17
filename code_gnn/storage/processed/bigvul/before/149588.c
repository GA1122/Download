void LoadingPredictorPreconnectTest::SetUp() {
  LoadingPredictorTest::SetUp();
  auto mock_preconnect_manager =
      std::make_unique<StrictMock<MockPreconnectManager>>(
          predictor_->GetWeakPtr(), profile_.get());
  mock_preconnect_manager_ = mock_preconnect_manager.get();
  predictor_->set_mock_preconnect_manager(std::move(mock_preconnect_manager));
}
