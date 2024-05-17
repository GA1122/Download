void ExtensionPrefs::SetDelaysNetworkRequests(const std::string& extension_id,
                                              bool does_delay) {
  if (does_delay) {
    UpdateExtensionPref(extension_id, kPrefDelayNetworkRequests,
                        Value::CreateBooleanValue(true));
  } else {
    UpdateExtensionPref(extension_id, kPrefDelayNetworkRequests, NULL);
  }
}
