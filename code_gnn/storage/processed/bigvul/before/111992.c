void SyncTest::InitializeInstance(int index) {
  profiles_[index] = MakeProfile(
      base::StringPrintf(FILE_PATH_LITERAL("Profile%d"), index));
  EXPECT_FALSE(GetProfile(index) == NULL) << "Could not create Profile "
                                          << index << ".";

  browsers_[index] = Browser::Create(GetProfile(index));
  EXPECT_FALSE(GetBrowser(index) == NULL) << "Could not create Browser "
                                          << index << ".";

  clients_[index] = new ProfileSyncServiceHarness(GetProfile(index),
                                                  username_,
                                                  password_);
  EXPECT_FALSE(GetClient(index) == NULL) << "Could not create Client "
                                         << index << ".";

  ui_test_utils::WaitForBookmarkModelToLoad(
      GetProfile(index)->GetBookmarkModel());

  ui_test_utils::WaitForTemplateURLServiceToLoad(
      TemplateURLServiceFactory::GetForProfile(GetProfile(index)));
}
