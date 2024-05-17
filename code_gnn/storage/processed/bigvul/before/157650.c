std::string ExtensionTabsZoomTest::RunSetZoomSettingsExpectError(
    int tab_id,
    const char* mode,
    const char* scope) {
  scoped_refptr<TabsSetZoomSettingsFunction> set_zoom_settings_function(
      new TabsSetZoomSettingsFunction());
  set_zoom_settings_function->set_extension(extension_.get());

  return utils::RunFunctionAndReturnError(set_zoom_settings_function.get(),
                                          base::StringPrintf(
                                              "[%u, {\"mode\": \"%s\", "
                                              "\"scope\": \"%s\"}]",
                                              tab_id,
                                              mode,
                                              scope),
                                          browser());
}
