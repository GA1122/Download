void DevToolsUIBindings::DeviceCountChanged(int count) {
  base::FundamentalValue value(count);
  CallClientFunction("DevToolsAPI.deviceCountUpdated", &value, NULL,
                     NULL);
}
