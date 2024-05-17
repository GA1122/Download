void ChromeRenderMessageFilter::OnExtensionUnloadAck(
     const std::string& extension_id) {
  if (profile_->GetExtensionProcessManager())
    profile_->GetExtensionProcessManager()->OnUnloadAck(extension_id);
}
