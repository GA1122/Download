void RenderThread::OnExtensionSetAPIPermissions(
    const std::string& extension_id,
    const std::vector<std::string>& permissions) {
  ExtensionProcessBindings::SetAPIPermissions(extension_id, permissions);
}
