bool Extension::LoadContentScripts(string16* error) {
  if (!manifest_->HasKey(keys::kContentScripts))
    return true;
  ListValue* list_value;
  if (!manifest_->GetList(keys::kContentScripts, &list_value)) {
    *error = ASCIIToUTF16(errors::kInvalidContentScriptsList);
    return false;
  }

  for (size_t i = 0; i < list_value->GetSize(); ++i) {
    DictionaryValue* content_script = NULL;
    if (!list_value->GetDictionary(i, &content_script)) {
      *error = ErrorUtils::FormatErrorMessageUTF16(
          errors::kInvalidContentScript, base::IntToString(i));
      return false;
    }

    UserScript script;
    if (!LoadUserScriptHelper(content_script, i, error, &script))
      return false;   
    script.set_extension_id(id());
    if (converted_from_user_script_) {
      script.set_emulate_greasemonkey(true);
      script.set_match_all_frames(true);   
    }
    content_scripts_.push_back(script);
  }
  return true;
}
