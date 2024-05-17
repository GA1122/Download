  static v8::Handle<v8::Value> IsExtensionProcess(const v8::Arguments& args) {
    ExtensionImpl* v8_extension = GetFromArguments<ExtensionImpl>(args);
    return v8::Boolean::New(
        v8_extension->extension_dispatcher_->is_extension_process());
  }
