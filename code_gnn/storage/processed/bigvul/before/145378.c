std::unique_ptr<NativeHandler> V8SchemaRegistry::AsNativeHandler() {
  std::unique_ptr<ScriptContext> context(
      new ScriptContext(GetOrCreateContext(v8::Isolate::GetCurrent()),
                        NULL,   
                        NULL,   
                        Feature::UNSPECIFIED_CONTEXT,
                        NULL,   
                        Feature::UNSPECIFIED_CONTEXT));
  return std::unique_ptr<NativeHandler>(
      new SchemaRegistryNativeHandler(this, std::move(context)));
}