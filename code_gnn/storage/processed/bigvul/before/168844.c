ExtensionFunction::ResponseAction TabsDiscardFunction::Run() {
  std::unique_ptr<tabs::Discard::Params> params(
      tabs::Discard::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params);

  WebContents* contents = nullptr;
  if (params->tab_id) {
    int tab_id = *params->tab_id;
    std::string error;
    if (!GetTabById(tab_id, browser_context(), include_incognito(), nullptr,
                    nullptr, &contents, nullptr, &error)) {
      return RespondNow(Error(error));
    }
  }
  contents =
      g_browser_process->GetTabManager()->DiscardTabByExtension(contents);

  if (contents) {
    return RespondNow(ArgumentList(
        tabs::Discard::Results::Create(*ExtensionTabUtil::CreateTabObject(
            contents, ExtensionTabUtil::kScrubTab, extension()))));
  }

  return RespondNow(Error(
      params->tab_id
          ? ErrorUtils::FormatErrorMessage(keys::kCannotDiscardTab,
                                           base::IntToString(*params->tab_id))
          : keys::kCannotFindTabToDiscard));
}
