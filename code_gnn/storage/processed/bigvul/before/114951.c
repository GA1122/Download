TabContentsWrapper* GetTabContentsWrapperFromDict(const Browser* browser,
                                                  const DictionaryValue* args,
                                                  std::string* error_message) {
  int tab_index;
  if (!args->GetInteger("tab_index", &tab_index)) {
    *error_message = "Must include tab_index.";
    return NULL;
  }

  TabContentsWrapper* tab_contents =
      browser->GetTabContentsWrapperAt(tab_index);
  if (!tab_contents) {
    *error_message = StringPrintf("No tab at index %d.", tab_index);
    return NULL;
  }
  return tab_contents;
}
