ExtensionFunction::ResponseAction TabsGetFunction::Run() {
  std::unique_ptr<tabs::Get::Params> params(tabs::Get::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());
  int tab_id = params->tab_id;

  TabStripModel* tab_strip = NULL;
  WebContents* contents = NULL;
  int tab_index = -1;
  std::string error;
  if (!GetTabById(tab_id, browser_context(), include_incognito_information(),
                  NULL, &tab_strip, &contents, &tab_index, &error)) {
    return RespondNow(Error(error));
  }

  return RespondNow(ArgumentList(tabs::Get::Results::Create(
      *ExtensionTabUtil::CreateTabObject(contents, ExtensionTabUtil::kScrubTab,
                                         extension(), tab_strip, tab_index))));
}
