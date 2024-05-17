void ExtensionSettingsHandler::ShowAlert(const std::string& message) {
  ListValue arguments;
  arguments.Append(Value::CreateStringValue(message));
  web_ui()->CallJavascriptFunction("alert", arguments);
}
