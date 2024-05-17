  void CreateTestAutofillProfiles() {
    AutofillProfile profile1;
    test::SetProfileInfo(&profile1, "Elvis", "Aaron", "Presley",
                         "theking@gmail.com", "RCA", "3734 Elvis Presley Blvd.",
                         "Apt. 10", "Memphis", "Tennessee", "38116", "US",
                         "12345678901");
    profile1.set_guid("00000000-0000-0000-0000-000000000001");
    personal_data_.AddProfile(profile1);

    AutofillProfile profile2;
    test::SetProfileInfo(&profile2, "Charles", "Hardin", "Holley",
                         "buddy@gmail.com", "Decca", "123 Apple St.", "unit 6",
                         "Lubbock", "Texas", "79401", "US", "23456789012");
    profile2.set_guid("00000000-0000-0000-0000-000000000002");
    personal_data_.AddProfile(profile2);

    AutofillProfile profile3;
    test::SetProfileInfo(&profile3, "", "", "", "", "", "", "", "", "", "", "",
                         "");
    profile3.set_guid("00000000-0000-0000-0000-000000000003");
    personal_data_.AddProfile(profile3);
  }
