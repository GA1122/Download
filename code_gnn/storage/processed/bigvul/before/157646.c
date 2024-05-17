testing::AssertionResult ExtensionTabsZoomTest::RunGetZoomSettings(
    int tab_id,
    std::string* mode,
    std::string* scope) {
  DCHECK(mode);
  DCHECK(scope);
  scoped_refptr<TabsGetZoomSettingsFunction> get_zoom_settings_function(
      new TabsGetZoomSettingsFunction());
  get_zoom_settings_function->set_extension(extension_.get());
  get_zoom_settings_function->set_has_callback(true);

  std::unique_ptr<base::DictionaryValue> get_zoom_settings_result(
      utils::ToDictionary(utils::RunFunctionAndReturnSingleResult(
          get_zoom_settings_function.get(), base::StringPrintf("[%u]", tab_id),
          browser())));

  if (!get_zoom_settings_result)
    return testing::AssertionFailure() << "no result";

  *mode = api_test_utils::GetString(get_zoom_settings_result.get(), "mode");
  *scope = api_test_utils::GetString(get_zoom_settings_result.get(), "scope");

  return testing::AssertionSuccess();
}
