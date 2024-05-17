bool GetAllTabsInWindowFunction::RunImpl() {
  Browser* browser;
  int window_id = -1;
  if (HasOptionalArgument(0)) {
    EXTENSION_FUNCTION_VALIDATE(args_->GetInteger(0, &window_id));
    browser = GetBrowserInProfileWithId(profile(), window_id,
                                        include_incognito(), &error_);
  } else {
    browser = GetCurrentBrowser();
    if (!browser)
      error_ = keys::kNoCurrentWindowError;
  }
  if (!browser)
    return false;

  result_.reset(ExtensionTabUtil::CreateTabList(browser));

  return true;
}
