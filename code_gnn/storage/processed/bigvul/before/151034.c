void DevToolsUIBindings::SetDevicesUpdatesEnabled(bool enabled) {
  if (devices_updates_enabled_ == enabled)
    return;
  devices_updates_enabled_ = enabled;
  if (enabled) {
    remote_targets_handler_ = DevToolsTargetsUIHandler::CreateForAdb(
        base::Bind(&DevToolsUIBindings::DevicesUpdated,
                   base::Unretained(this)),
        profile_);
    pref_change_registrar_.Init(profile_->GetPrefs());
    pref_change_registrar_.Add(prefs::kDevToolsDiscoverUsbDevicesEnabled,
        base::Bind(&DevToolsUIBindings::DevicesDiscoveryConfigUpdated,
                   base::Unretained(this)));
    pref_change_registrar_.Add(prefs::kDevToolsPortForwardingEnabled,
        base::Bind(&DevToolsUIBindings::DevicesDiscoveryConfigUpdated,
                   base::Unretained(this)));
    pref_change_registrar_.Add(prefs::kDevToolsPortForwardingConfig,
        base::Bind(&DevToolsUIBindings::DevicesDiscoveryConfigUpdated,
                   base::Unretained(this)));
    port_status_serializer_.reset(new PortForwardingStatusSerializer(
        base::Bind(&DevToolsUIBindings::SendPortForwardingStatus,
                   base::Unretained(this)),
        profile_));
    DevicesDiscoveryConfigUpdated();
  } else {
    remote_targets_handler_.reset();
    port_status_serializer_.reset();
    pref_change_registrar_.RemoveAll();
    SendPortForwardingStatus(base::DictionaryValue());
  }
}
