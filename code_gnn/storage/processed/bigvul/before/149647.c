void ResourcePrefetchPredictorTest::TearDown() {
  EXPECT_EQ(*predictor_->host_redirect_data_->data_cache_,
            mock_tables_->host_redirect_table_.data_);
  EXPECT_EQ(*predictor_->origin_data_->data_cache_,
            mock_tables_->origin_table_.data_);
  loading_predictor_->Shutdown();
}
