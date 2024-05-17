void ExtensionHelper::OnExtensionResponse(int request_id,
                                          bool success,
                                          const std::string& response,
                                          const std::string& error) {
  std::string extension_id;
  SchemaGeneratedBindings::HandleResponse(
      extension_dispatcher_->v8_context_set(), request_id, success,
      response, error, &extension_id);

  if (CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableLazyBackgroundPages))
    extension_dispatcher_->CheckIdleStatus(extension_id);
}
