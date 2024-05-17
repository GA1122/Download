void CallModuleMethod(const std::string& module_name,
                      const std::string& method_name,
                      const base::ListValue* args,
                      ScriptContext* context) {
  v8::HandleScope handle_scope(context->isolate());
  v8::Context::Scope context_scope(context->v8_context());

  scoped_ptr<content::V8ValueConverter> converter(
      content::V8ValueConverter::create());

  std::vector<v8::Local<v8::Value>> arguments;
  for (base::ListValue::const_iterator it = args->begin(); it != args->end();
       ++it) {
    arguments.push_back(converter->ToV8Value(*it, context->v8_context()));
  }

  context->module_system()->CallModuleMethod(
      module_name, method_name, &arguments);
}
