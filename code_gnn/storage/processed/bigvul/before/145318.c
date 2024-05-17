void V8ContextNativeHandler::GetModuleSystem(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK_EQ(args.Length(), 1);
  CHECK(args[0]->IsObject());
  ScriptContext* context = ScriptContextSet::GetContextByObject(
      v8::Local<v8::Object>::Cast(args[0]));
  if (blink::WebFrame::scriptCanAccess(context->web_frame()))
    args.GetReturnValue().Set(context->module_system()->NewInstance());
}
