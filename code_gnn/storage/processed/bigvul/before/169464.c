void NetworkChangeNotifierMac::Forwarder::SetDynamicStoreNotificationKeys(
    SCDynamicStoreRef store)  {
  net_config_watcher_->SetDynamicStoreNotificationKeys(store);
}
