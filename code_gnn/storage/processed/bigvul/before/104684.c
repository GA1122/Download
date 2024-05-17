  static v8::Handle<v8::Value> GetExtensionAPIDefinition(
      const v8::Arguments& args) {
    return v8::String::New(GetStringResource(IDR_EXTENSION_API_JSON));
  }
