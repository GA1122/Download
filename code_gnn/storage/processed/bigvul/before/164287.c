void ActiveTabWithServiceTest::TearDown() {
  content::BrowserSideNavigationTearDown();
  ExtensionServiceTestBase::TearDown();
}
