void AutofillMetricsTest::TearDown() {
  autofill_manager_.reset();
  autofill_driver_.reset();
  personal_data_.reset();
  test::ReenableSystemServices();
  test_ukm_recorder_.Purge();
}
