void DevToolsUIBindings::AppendedTo(const std::string& url) {
  base::StringValue url_value(url);
  CallClientFunction("DevToolsAPI.appendedToURL", &url_value, NULL,
                     NULL);
}
