void AutofillMetricsTest::CreateAmbiguousProfiles() {
  personal_data_->ClearProfiles();
  CreateTestAutofillProfiles();

  AutofillProfile profile;
  test::SetProfileInfo(&profile, "John", "Decca", "Public", "john@gmail.com",
                       "Company", "123 Main St.", "unit 7", "Springfield",
                       "Texas", "79401", "US", "2345678901");
  profile.set_guid("00000000-0000-0000-0000-000000000003");
  personal_data_->AddProfile(profile);
  personal_data_->Refresh();
}
