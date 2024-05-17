bool FileManagerBrowserTestBase::SetUpUserDataDirectory() {
  if (IsGuestModeTest())
    return true;

  return drive::SetUpUserDataDirectoryForDriveFsTest();
}
