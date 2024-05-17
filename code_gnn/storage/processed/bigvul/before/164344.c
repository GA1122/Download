ExtensionFunction::ResponseAction TabsGetAllInWindowFunction::Run() {
  std::unique_ptr<tabs::GetAllInWindow::Params> params(
      tabs::GetAllInWindow::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());
  int window_id = extension_misc::kCurrentWindowId;
  if (params->window_id.get())
    window_id = *params->window_id;

  Browser* browser = NULL;
  std::string error;
  if (!GetBrowserFromWindowID(this, window_id, &browser, &error))
    return RespondNow(Error(error));

  return RespondNow(
      OneArgument(ExtensionTabUtil::CreateTabList(browser, extension())));
}
