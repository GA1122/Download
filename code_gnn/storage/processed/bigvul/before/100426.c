void RenderThread::OnExtensionSetL10nMessages(
    const std::string& extension_id,
    const std::map<std::string, std::string>& l10n_messages) {
  ExtensionProcessBindings::SetL10nMessages(extension_id, l10n_messages);
}
