void AutofillMetricsTest::RecreateProfile() {
  personal_data_->ClearProfiles();

  AutofillProfile profile;
  test::SetProfileInfo(&profile, "Elvis", "Aaron", "Presley",
                       "theking@gmail.com", "RCA", "3734 Elvis Presley Blvd.",
                       "Apt. 10", "Memphis", "Tennessee", "38116", "US",
                       "12345678901");
  profile.set_guid("00000000-0000-0000-0000-000000000001");
  personal_data_->AddProfile(profile);
  personal_data_->Refresh();
}
