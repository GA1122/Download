void DisplaySourceCustomBindings::OnCallCompleted(
    int call_id,
    bool success,
    const std::string& error_message) {
  v8::Isolate* isolate = context()->isolate();
  ModuleSystem* module_system = context()->module_system();
  v8::HandleScope handle_scope(isolate);
  v8::Context::Scope context_scope(context()->v8_context());

  v8::Local<v8::Value> callback_args[2];
  callback_args[0] = v8::Integer::New(isolate, call_id);
  if (success)
    callback_args[1] = v8::Null(isolate);
  else
    callback_args[1] = v8::String::NewFromUtf8(isolate, error_message.c_str());

  module_system->CallModuleMethod("displaySource", "callCompletionCallback", 2,
                                  callback_args);
}
