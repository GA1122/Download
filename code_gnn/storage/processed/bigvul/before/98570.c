bool GetLastFocusedWindowFunction::RunImpl() {
  Browser* browser = BrowserList::FindBrowserWithType(
      profile(), Browser::TYPE_ANY, include_incognito());
  if (!browser) {
    error_ = keys::kNoLastFocusedWindowError;
    return false;
  }
  result_.reset(ExtensionTabUtil::CreateWindowValue(browser, false));
  return true;
}
