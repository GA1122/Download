void ChromeRenderMessageFilter::OnExtensionShouldUnloadAck(
     const std::string& extension_id, int sequence_id) {
  if (profile_->GetExtensionProcessManager())
    profile_->GetExtensionProcessManager()->OnShouldUnloadAck(
        extension_id, sequence_id);
}
