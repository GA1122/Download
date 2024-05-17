void Dispatcher::OnLoaded(
    const std::vector<ExtensionMsg_Loaded_Params>& loaded_extensions) {
  for (const auto& param : loaded_extensions) {
    std::string error;
    scoped_refptr<const Extension> extension = param.ConvertToExtension(&error);
    if (!extension.get()) {
      NOTREACHED() << error;
      extension_load_errors_[param.id] = error;
      continue;
    }

    RendererExtensionRegistry* extension_registry =
        RendererExtensionRegistry::Get();
    if (!extension_registry->Contains(extension->id()))
      extension_registry->Insert(extension);
  }

  UpdateBindings("");
}
