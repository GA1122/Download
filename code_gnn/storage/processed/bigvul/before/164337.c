ExtensionFunction::ResponseAction WindowsGetCurrentFunction::Run() {
  std::unique_ptr<windows::GetCurrent::Params> params(
      windows::GetCurrent::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  ApiParameterExtractor<windows::GetCurrent::Params> extractor(params.get());
  Browser* browser = nullptr;
  std::string error;
  if (!windows_util::GetBrowserFromWindowID(
          this, extension_misc::kCurrentWindowId, extractor.type_filters(),
          &browser, &error)) {
    return RespondNow(Error(error));
  }

  ExtensionTabUtil::PopulateTabBehavior populate_tab_behavior =
      extractor.populate_tabs() ? ExtensionTabUtil::kPopulateTabs
                                : ExtensionTabUtil::kDontPopulateTabs;
  std::unique_ptr<base::DictionaryValue> windows =
      ExtensionTabUtil::CreateWindowValueForExtension(*browser, extension(),
                                                      populate_tab_behavior);
  return RespondNow(OneArgument(std::move(windows)));
}
