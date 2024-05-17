void Warn(v8::Isolate* isolate, const std::string& message) {
  ScriptContext* script_context =
      ScriptContextSet::GetContextByV8Context(isolate->GetCallingContext());
  console::Warn(script_context ? script_context->GetRenderFrame() : nullptr,
                message);
}
