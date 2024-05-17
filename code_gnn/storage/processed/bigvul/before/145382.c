   SchemaRegistryNativeHandler(V8SchemaRegistry* registry,
                              std::unique_ptr<ScriptContext> context)
      : ObjectBackedNativeHandler(context.get()),
        context_(std::move(context)),
        registry_(registry) {
    RouteFunction("GetSchema",
                  base::Bind(&SchemaRegistryNativeHandler::GetSchema,
                             base::Unretained(this)));
  }
