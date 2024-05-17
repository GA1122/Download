bool NetworkChangeNotifier::IsOffline() {
  return g_network_change_notifier &&
         g_network_change_notifier->IsCurrentlyOffline();
}
