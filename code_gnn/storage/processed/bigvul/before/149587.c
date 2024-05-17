void LoadingPredictorTest::SetUp() {
  profile_ = std::make_unique<TestingProfile>();
  SetPreference();
  auto config = CreateConfig();
  predictor_ = std::make_unique<LoadingPredictor>(config, profile_.get());

  auto mock = std::make_unique<StrictMock<MockResourcePrefetchPredictor>>(
      config, profile_.get());
  EXPECT_CALL(*mock, PredictPreconnectOrigins(GURL(kUrl), _))
      .WillRepeatedly(Return(true));
  EXPECT_CALL(*mock, PredictPreconnectOrigins(GURL(kUrl2), _))
      .WillRepeatedly(Return(true));
  EXPECT_CALL(*mock, PredictPreconnectOrigins(GURL(kUrl3), _))
      .WillRepeatedly(Return(false));

  mock_predictor_ = mock.get();
  predictor_->set_mock_resource_prefetch_predictor(std::move(mock));

  predictor_->StartInitialization();
  content::RunAllTasksUntilIdle();
}
