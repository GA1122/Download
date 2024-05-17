bool GetBrowserFromWindowID(UIThreadExtensionFunction* function,
                            int window_id,
                            Browser** browser,
                            std::string* error) {
  return GetBrowserFromWindowID(ChromeExtensionFunctionDetails(function),
                                window_id, browser, error);
}
