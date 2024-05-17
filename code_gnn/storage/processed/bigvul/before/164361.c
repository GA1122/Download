ExtensionFunction::ResponseAction TabsGoForwardFunction::Run() {
  std::unique_ptr<tabs::GoForward::Params> params(
      tabs::GoForward::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  int tab_id = params->tab_id ? *params->tab_id : -1;
  std::string error;
  WebContents* web_contents =
      GetTabsAPIDefaultWebContents(this, tab_id, &error);
  if (!web_contents)
    return RespondNow(Error(error));

  NavigationController& controller = web_contents->GetController();
  if (!controller.CanGoForward())
    return RespondNow(Error(tabs_constants::kNotFoundNextPageError));

  controller.GoForward();
  return RespondNow(NoArguments());
}
