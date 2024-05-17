bool GetAllWindowsFunction::RunImpl() {
  bool populate_tabs = false;
  if (HasOptionalArgument(0)) {
    DictionaryValue* args;
    EXTENSION_FUNCTION_VALIDATE(args_->GetDictionary(0, &args));

    if (args->HasKey(keys::kPopulateKey)) {
      EXTENSION_FUNCTION_VALIDATE(args->GetBoolean(keys::kPopulateKey,
          &populate_tabs));
    }
  }

  result_.reset(new ListValue());
  Profile* incognito_profile =
      include_incognito() ? profile()->GetOffTheRecordProfile() : NULL;
  for (BrowserList::const_iterator browser = BrowserList::begin();
    browser != BrowserList::end(); ++browser) {
      if ((*browser)->profile() == profile() ||
          (*browser)->profile() == incognito_profile) {
        static_cast<ListValue*>(result_.get())->
          Append(ExtensionTabUtil::CreateWindowValue(*browser, populate_tabs));
      }
  }

  return true;
}
