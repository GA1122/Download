void ExtensionSystemImpl::Shared::Shutdown() {
  if (extension_warning_service_.get()) {
    extension_warning_service_->RemoveObserver(
        extension_warning_badge_service_.get());
  }
  if (extension_service_.get())
    extension_service_->Shutdown();
}
