bool ExtensionApiTest::RunExtensionTestImpl(const std::string& extension_name,
                                            const std::string& page_url,
                                            const char* custom_arg,
                                            int flags) {
  bool load_as_component = (flags & kFlagLoadAsComponent) != 0;
  bool launch_platform_app = (flags & kFlagLaunchPlatformApp) != 0;
  bool use_incognito = (flags & kFlagUseIncognito) != 0;
  bool use_root_extensions_dir = (flags & kFlagUseRootExtensionsDir) != 0;

  if (custom_arg && custom_arg[0])
    SetCustomArg(custom_arg);

  ResultCatcher catcher;
  DCHECK(!extension_name.empty() || !page_url.empty()) <<
      "extension_name and page_url cannot both be empty";

  const Extension* extension = NULL;
  if (!extension_name.empty()) {
    const base::FilePath& root_path =
        use_root_extensions_dir ? shared_test_data_dir_ : test_data_dir_;
    base::FilePath extension_path = root_path.AppendASCII(extension_name);
    if (load_as_component) {
      extension = LoadExtensionAsComponent(extension_path);
    } else {
      int browser_test_flags = ExtensionBrowserTest::kFlagNone;
      if (flags & kFlagEnableIncognito)
        browser_test_flags |= ExtensionBrowserTest::kFlagEnableIncognito;
      if (flags & kFlagEnableFileAccess)
        browser_test_flags |= ExtensionBrowserTest::kFlagEnableFileAccess;
      if (flags & kFlagIgnoreManifestWarnings)
        browser_test_flags |= ExtensionBrowserTest::kFlagIgnoreManifestWarnings;
      if (flags & kFlagAllowOldManifestVersions) {
        browser_test_flags |=
            ExtensionBrowserTest::kFlagAllowOldManifestVersions;
      }
      extension = LoadExtensionWithFlags(extension_path, browser_test_flags);
    }
    if (!extension) {
      message_ = "Failed to load extension.";
      return false;
    }
  }

  if (!page_url.empty()) {
    GURL url = GURL(page_url);

    if (!url.is_valid()) {
      DCHECK(!extension_name.empty()) <<
          "Relative page_url given with no extension_name";

      url = extension->GetResourceURL(page_url);
    }

    if (use_incognito)
      OpenURLOffTheRecord(browser()->profile(), url);
    else
      ui_test_utils::NavigateToURL(browser(), url);
  } else if (launch_platform_app) {
    AppLaunchParams params(browser()->profile(), extension,
                           LAUNCH_CONTAINER_NONE,
                           WindowOpenDisposition::NEW_WINDOW, SOURCE_TEST);
    params.command_line = *base::CommandLine::ForCurrentProcess();
    OpenApplication(params);
  }

  if (!catcher.GetNextResult()) {
    message_ = catcher.message();
    return false;
  }

  return true;
}
