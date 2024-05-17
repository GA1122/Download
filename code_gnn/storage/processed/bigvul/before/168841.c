ExtensionFunction::ResponseAction TabsDuplicateFunction::Run() {
  std::unique_ptr<tabs::Duplicate::Params> params(
      tabs::Duplicate::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());
  int tab_id = params->tab_id;

  Browser* browser = NULL;
  TabStripModel* tab_strip = NULL;
  int tab_index = -1;
  std::string error;
  if (!GetTabById(tab_id, browser_context(), include_incognito(), &browser,
                  &tab_strip, NULL, &tab_index, &error)) {
    return RespondNow(Error(error));
  }

  WebContents* new_contents = chrome::DuplicateTabAt(browser, tab_index);
  if (!has_callback())
    return RespondNow(NoArguments());

  TabStripModel* new_tab_strip = NULL;
  int new_tab_index = -1;
  ExtensionTabUtil::GetTabStripModel(new_contents,
                                     &new_tab_strip,
                                     &new_tab_index);
  if (!new_tab_strip || new_tab_index == -1) {
    return RespondNow(Error(kUnknownErrorDoNotUse));
  }

  return RespondNow(ArgumentList(
      tabs::Get::Results::Create(*ExtensionTabUtil::CreateTabObject(
          new_contents, ExtensionTabUtil::kScrubTab, extension(), new_tab_strip,
          new_tab_index))));
}
