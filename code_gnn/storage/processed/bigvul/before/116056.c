void ExtensionSettingsHandler::FileSelected(const FilePath& path, int index,
                                            void* params) {
  ListValue results;
  results.Append(Value::CreateStringValue(path.value()));
  web_ui()->CallJavascriptFunction("window.handleFilePathSelected", results);
}
