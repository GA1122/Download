void WebUIExtension::Install(blink::WebLocalFrame* frame) {
  v8::Isolate* isolate = blink::MainThreadIsolate();
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::Context> context = frame->MainWorldScriptContext();
  if (context.IsEmpty())
    return;

  v8::Context::Scope context_scope(context);

  v8::Local<v8::Object> chrome = GetOrCreateChromeObject(isolate,
                                                          context->Global());
  chrome->Set(gin::StringToSymbol(isolate, "send"),
              gin::CreateFunctionTemplate(
                  isolate, base::Bind(&WebUIExtension::Send))->GetFunction());
  chrome->Set(gin::StringToSymbol(isolate, "getVariableValue"),
              gin::CreateFunctionTemplate(
                  isolate, base::Bind(&WebUIExtension::GetVariableValue))
                  ->GetFunction());
}
