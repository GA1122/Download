void DevToolsUIBindings::DevicesUpdated(
    const std::string& source,
    const base::ListValue& targets) {
  CallClientFunction("DevToolsAPI.devicesUpdated", &targets, NULL,
                     NULL);
}
