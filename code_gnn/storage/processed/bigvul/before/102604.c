    BrowserPolicyConnector::GetDeviceCloudPolicyDataStore() const {
#if defined(OS_CHROMEOS)
  return device_data_store_.get();
#else
  return NULL;
#endif
}
