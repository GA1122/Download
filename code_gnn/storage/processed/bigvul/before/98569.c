bool GetCurrentWindowFunction::RunImpl() {
  Browser* browser = GetCurrentBrowser();
  if (!browser) {
    error_ = keys::kNoCurrentWindowError;
    return false;
  }
  result_.reset(ExtensionTabUtil::CreateWindowValue(browser, false));
  return true;
}
