void AutofillMetricsTest::SetUp() {
  autofill_client_.SetPrefs(test::PrefServiceForTesting());

  personal_data_ = std::make_unique<TestPersonalDataManager>();
  personal_data_->set_database(autofill_client_.GetDatabase());
  personal_data_->SetPrefService(autofill_client_.GetPrefs());
  personal_data_->SetSyncServiceForTest(&sync_service_);
  autofill_driver_ = std::make_unique<TestAutofillDriver>();
  autofill_manager_ = std::make_unique<TestAutofillManager>(
      autofill_driver_.get(), &autofill_client_, personal_data_.get());

  external_delegate_ = std::make_unique<AutofillExternalDelegate>(
      autofill_manager_.get(), autofill_driver_.get());
  autofill_manager_->SetExternalDelegate(external_delegate_.get());

  CreateTestAutofillProfiles();
}
