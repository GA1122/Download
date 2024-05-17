ObjectBackedNativeHandler::ObjectBackedNativeHandler(ScriptContext* context)
    : router_data_(context->isolate()),
      context_(context),
      object_template_(context->isolate(),
                       v8::ObjectTemplate::New(context->isolate())) {
}
