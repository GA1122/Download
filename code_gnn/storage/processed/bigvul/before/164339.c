ExtensionFunction::ResponseAction WindowsGetAllFunction::Run() {
  std::unique_ptr<windows::GetAll::Params> params(
      windows::GetAll::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  ApiParameterExtractor<windows::GetAll::Params> extractor(params.get());
  std::unique_ptr<base::ListValue> window_list(new base::ListValue());
  ExtensionTabUtil::PopulateTabBehavior populate_tab_behavior =
      extractor.populate_tabs() ? ExtensionTabUtil::kPopulateTabs
                                : ExtensionTabUtil::kDontPopulateTabs;
  for (auto* controller : WindowControllerList::GetInstance()->windows()) {
    if (!controller->GetBrowser() ||
        !windows_util::CanOperateOnWindow(this, controller,
                                          extractor.type_filters())) {
      continue;
    }
    window_list->Append(ExtensionTabUtil::CreateWindowValueForExtension(
        *controller->GetBrowser(), extension(), populate_tab_behavior));
  }

  return RespondNow(OneArgument(std::move(window_list)));
}
