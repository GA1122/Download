void ChromeRenderMessageFilter::OnExtensionRemoveLazyListener(
    const std::string& extension_id, const std::string& event_name) {
  if (profile_->GetExtensionEventRouter())
    profile_->GetExtensionEventRouter()->RemoveLazyEventListener(
        event_name, extension_id);
}
