bool GetBrowserFromWindowID(const ChromeExtensionFunctionDetails& details,
                            int window_id,
                            Browser** browser,
                            std::string* error) {
  Browser* result = nullptr;
  result = ExtensionTabUtil::GetBrowserFromWindowID(details, window_id, error);
  if (!result)
    return false;

  *browser = result;
  return true;
}
