void LoadingStatsCollectorTest::SetUp() {
  LoadingPredictorConfig config;
  PopulateTestConfig(&config);
  profile_ = std::make_unique<TestingProfile>();
  content::RunAllTasksUntilIdle();
  mock_predictor_ = std::make_unique<StrictMock<MockResourcePrefetchPredictor>>(
      config, profile_.get());
  stats_collector_ =
      std::make_unique<LoadingStatsCollector>(mock_predictor_.get(), config);
  histogram_tester_ = std::make_unique<base::HistogramTester>();
  content::RunAllTasksUntilIdle();
}
