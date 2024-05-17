void DevToolsUIBindings::CanceledFileSaveAs(const std::string& url) {
  base::StringValue url_value(url);
  CallClientFunction("DevToolsAPI.canceledSaveURL",
                     &url_value, NULL, NULL);
}
