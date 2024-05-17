void NetworkChangeNotifierMac::Forwarder::OnNetworkConfigChange(
    CFArrayRef changed_keys)  {
  net_config_watcher_->OnNetworkConfigChange(changed_keys);
}
