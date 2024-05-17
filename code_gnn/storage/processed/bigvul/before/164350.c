ExtensionFunction::ResponseAction TabsHighlightFunction::Run() {
  std::unique_ptr<tabs::Highlight::Params> params(
      tabs::Highlight::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  int window_id = extension_misc::kCurrentWindowId;
  if (params->highlight_info.window_id.get())
    window_id = *params->highlight_info.window_id;

  Browser* browser = NULL;
  std::string error;
  if (!GetBrowserFromWindowID(this, window_id, &browser, &error))
    return RespondNow(Error(error));

  TabStripModel* tabstrip = browser->tab_strip_model();
  ui::ListSelectionModel selection;
  int active_index = -1;

  if (params->highlight_info.tabs.as_integers) {
    std::vector<int>& tab_indices = *params->highlight_info.tabs.as_integers;
    for (size_t i = 0; i < tab_indices.size(); ++i) {
      if (!HighlightTab(tabstrip, &selection, &active_index, tab_indices[i],
                        &error)) {
        return RespondNow(Error(error));
      }
    }
  } else {
    EXTENSION_FUNCTION_VALIDATE(params->highlight_info.tabs.as_integer);
    if (!HighlightTab(tabstrip, &selection, &active_index,
                      *params->highlight_info.tabs.as_integer, &error)) {
      return RespondNow(Error(error));
    }
  }

  if (selection.empty())
    return RespondNow(Error(tabs_constants::kNoHighlightedTabError));

  selection.set_active(active_index);
  browser->tab_strip_model()->SetSelectionFromModel(std::move(selection));
  return RespondNow(OneArgument(ExtensionTabUtil::CreateWindowValueForExtension(
      *browser, extension(), ExtensionTabUtil::kPopulateTabs)));
}
