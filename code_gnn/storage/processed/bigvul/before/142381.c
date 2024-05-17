  static std::string GetFullTestCaseName(const TestCase& test) {
    std::string name(test.test_case_name);

    CHECK(!name.empty()) << "FATAL: no test case name.";

    if (test.guest_mode == IN_GUEST_MODE)
      name.append("_GuestMode");
    else if (test.guest_mode == IN_INCOGNITO)
      name.append("_Incognito");

    if (test.tablet_mode)
      name.append("_TabletMode");

    if (test.enable_drivefs.value_or(false))
      name.append("_DriveFs");

    if (!test.enable_native_smb)
      name.append("_DisableNativeSmb");

    if (test.enable_myfiles_volume.value_or(false))
      name.append("_MyFilesVolume");

    return name;
  }
