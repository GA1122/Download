void ExtensionPrefs::FixMissingPrefs(const ExtensionIdSet& extension_ids) {
  for (ExtensionIdSet::const_iterator ext_id = extension_ids.begin();
       ext_id != extension_ids.end(); ++ext_id) {
    if (GetInstallTime(*ext_id) == base::Time()) {
      LOG(INFO) << "Could not parse installation time of extension "
                << *ext_id << ". It was probably installed before setting "
                << kPrefInstallTime << " was introduced. Updating "
                << kPrefInstallTime << " to the current time.";
      const base::Time install_time = GetCurrentTime();
      UpdateExtensionPref(*ext_id, kPrefInstallTime, Value::CreateStringValue(
          base::Int64ToString(install_time.ToInternalValue())));
    }
  }
}
