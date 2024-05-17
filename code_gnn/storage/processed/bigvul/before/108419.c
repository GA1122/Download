void ChromeRenderMessageFilter::OnExtensionAddLazyListener(
    const std::string& extension_id, const std::string& event_name) {
  if (profile_->GetExtensionEventRouter())
    profile_->GetExtensionEventRouter()->AddLazyEventListener(
        event_name, extension_id);
}
