void AutomationProvider::UninstallExtension(int extension_handle,
                                            bool* success) {
  *success = false;
  const Extension* extension = GetExtension(extension_handle);
  ExtensionService* service = profile_->GetExtensionService();
  if (extension && service) {
    ExtensionUnloadNotificationObserver observer;
    service->UninstallExtension(extension->id(), false);
    *success = observer.did_receive_unload_notification();
  }
}
