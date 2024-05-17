  void ResetPredictor(bool small_db = true) {
    if (loading_predictor_)
      loading_predictor_->Shutdown();

    LoadingPredictorConfig config;
    PopulateTestConfig(&config, small_db);
    loading_predictor_ =
        std::make_unique<LoadingPredictor>(config, profile_.get());
    predictor_ = loading_predictor_->resource_prefetch_predictor();
    predictor_->set_mock_tables(mock_tables_);
  }
