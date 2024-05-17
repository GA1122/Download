void CheckRestrictedUrls(const Extension* extension,
                         bool block_chrome_urls) {
  const std::string& name = extension->name();
  const GURL chrome_settings_url("chrome://settings/");
  const GURL chrome_extension_url("chrome-extension://foo/bar.html");
  const GURL google_url("https://www.google.com/");
  const GURL self_url("chrome-extension://" + extension->id() + "/foo.html");
  const GURL invalid_url("chrome-debugger://foo/bar.html");

  std::string error;
  EXPECT_EQ(block_chrome_urls, extension->permissions_data()->IsRestrictedUrl(
                                   chrome_settings_url, &error))
      << name;
  if (block_chrome_urls)
    EXPECT_EQ(manifest_errors::kCannotAccessChromeUrl, error) << name;
  else
    EXPECT_TRUE(error.empty()) << name;

  error.clear();
  EXPECT_EQ(block_chrome_urls, extension->permissions_data()->IsRestrictedUrl(
                                   chrome_extension_url, &error))
      << name;
  if (block_chrome_urls)
    EXPECT_EQ(manifest_errors::kCannotAccessExtensionUrl, error) << name;
  else
    EXPECT_TRUE(error.empty()) << name;

  error.clear();
  EXPECT_FALSE(
      extension->permissions_data()->IsRestrictedUrl(google_url, &error))
      << name;
  EXPECT_TRUE(error.empty()) << name;

  error.clear();
  EXPECT_FALSE(extension->permissions_data()->IsRestrictedUrl(self_url, &error))
      << name;
  EXPECT_TRUE(error.empty()) << name;

  error.clear();
  bool allow_on_other_schemes = PermissionsData::CanExecuteScriptEverywhere(
      extension->id(), extension->location());
  EXPECT_EQ(!allow_on_other_schemes,
            extension->permissions_data()->IsRestrictedUrl(invalid_url, &error))
      << name;
  if (!allow_on_other_schemes) {
    EXPECT_EQ(ErrorUtils::FormatErrorMessage(
                  manifest_errors::kCannotAccessPage,
                  invalid_url.spec()),
              error) << name;
  } else {
    EXPECT_TRUE(error.empty());
  }
}
