void DevToolsUIBindings::FileSavedAs(const std::string& url) {
  base::StringValue url_value(url);
  CallClientFunction("DevToolsAPI.savedURL", &url_value, NULL, NULL);
}
