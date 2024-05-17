ExtensionFunction::ResponseAction WindowsGetLastFocusedFunction::Run() {
  std::unique_ptr<windows::GetLastFocused::Params> params(
      windows::GetLastFocused::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  ApiParameterExtractor<windows::GetLastFocused::Params> extractor(
      params.get());
  Browser* browser = nullptr;
  for (auto* controller : WindowControllerList::GetInstance()->windows()) {
    if (controller->GetBrowser() &&
        windows_util::CanOperateOnWindow(this, controller,
                                         extractor.type_filters())) {
      browser = controller->GetBrowser();
      if (controller->window()->IsActive())
        break;   
    }
  }
  if (!browser)
    return RespondNow(Error(tabs_constants::kNoLastFocusedWindowError));

  ExtensionTabUtil::PopulateTabBehavior populate_tab_behavior =
      extractor.populate_tabs() ? ExtensionTabUtil::kPopulateTabs
                                : ExtensionTabUtil::kDontPopulateTabs;
  std::unique_ptr<base::DictionaryValue> windows =
      ExtensionTabUtil::CreateWindowValueForExtension(*browser, extension(),
                                                      populate_tab_behavior);
  return RespondNow(OneArgument(std::move(windows)));
}
