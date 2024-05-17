void Dispatcher::RegisterNativeHandlers(ModuleSystem* module_system,
                                        ScriptContext* context) {
  RegisterNativeHandlers(module_system,
                         context,
                         this,
                         request_sender_.get(),
                         v8_schema_registry_.get());
  const Extension* extension = context->extension();
  int manifest_version = extension ? extension->manifest_version() : 1;
  bool is_component_extension =
      extension && Manifest::IsComponentLocation(extension->location());
  bool send_request_disabled =
      (extension && Manifest::IsUnpackedLocation(extension->location()) &&
       BackgroundInfo::HasLazyBackgroundPage(extension));
  module_system->RegisterNativeHandler(
      "process",
      scoped_ptr<NativeHandler>(new ProcessInfoNativeHandler(
          context,
          context->GetExtensionID(),
          context->GetContextTypeDescription(),
          ExtensionsRendererClient::Get()->IsIncognitoProcess(),
          is_component_extension,
          manifest_version,
          send_request_disabled)));

  delegate_->RegisterNativeHandlers(this, module_system, context);
}
