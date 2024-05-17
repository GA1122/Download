void TestingAutomationProvider::ImportSettings(Browser* browser,
                                               DictionaryValue* args,
                                               IPC::Message* reply_message) {
  std::map<std::string, importer::ImportItem> string_to_import_item;
  string_to_import_item["HISTORY"] = importer::HISTORY;
  string_to_import_item["FAVORITES"] = importer::FAVORITES;
  string_to_import_item["COOKIES"] = importer::COOKIES;
  string_to_import_item["PASSWORDS"] = importer::PASSWORDS;
  string_to_import_item["SEARCH_ENGINES"] = importer::SEARCH_ENGINES;
  string_to_import_item["HOME_PAGE"] = importer::HOME_PAGE;
  string_to_import_item["ALL"] = importer::ALL;

  ListValue* import_items_list = NULL;
  if (!args->GetString("import_from", &import_settings_data_.browser_name) ||
      !args->GetBoolean("first_run", &import_settings_data_.first_run) ||
      !args->GetList("import_items", &import_items_list)) {
    AutomationJSONReply(this, reply_message)
        .SendError("Incorrect type for one or more of the arguments.");
    return;
  }

  import_settings_data_.import_items = 0;
  int num_items = import_items_list->GetSize();
  for (int i = 0; i < num_items; i++) {
    std::string item;
    if (!import_items_list->GetString(i, &item) ||
        !ContainsKey(string_to_import_item, item)) {
      AutomationJSONReply(this, reply_message)
          .SendError("Invalid item string found in import_items.");
      return;
    }
    import_settings_data_.import_items |= string_to_import_item[item];
  }

  import_settings_data_.browser = browser;
  import_settings_data_.reply_message = reply_message;

  importer_list_ = new ImporterList(NULL);
  importer_list_->DetectSourceProfiles(this);
}
