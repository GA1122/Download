void AutomationProvider::DisableExtension(int extension_handle,
                                          bool* success) {
  *success = false;
  const Extension* extension = GetEnabledExtension(extension_handle);
  ExtensionService* service = profile_->GetExtensionService();
  if (extension && service) {
    ExtensionUnloadNotificationObserver observer;
    service->DisableExtension(extension->id());
    *success = observer.did_receive_unload_notification();
  }
}
