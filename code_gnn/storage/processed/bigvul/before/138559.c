void V8ContextNativeHandler::GetModuleSystem(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK_EQ(args.Length(), 1);
  CHECK(args[0]->IsObject());
  v8::Local<v8::Context> v8_context =
      v8::Local<v8::Object>::Cast(args[0])->CreationContext();
  ScriptContext* context =
      dispatcher_->script_context_set().GetByV8Context(v8_context);
  if (blink::WebFrame::scriptCanAccess(context->web_frame()))
     args.GetReturnValue().Set(context->module_system()->NewInstance());
 }
