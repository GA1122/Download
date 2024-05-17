  void CreateTestProfile() {
    AutofillProfile profile;
    autofill_test::SetProfileInfo(
        &profile, "Milton", "C.", "Waddams",
        "red.swingline@initech.com", "Initech", "4120 Freidrich Lane",
        "Basement", "Austin", "Texas", "78744", "United States", "5125551234");

    WindowedPersonalDataManagerObserver observer(browser());
    personal_data_manager()->AddProfile(profile);

    observer.Wait();
  }
