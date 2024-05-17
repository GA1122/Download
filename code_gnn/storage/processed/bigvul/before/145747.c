void ModuleSystem::RequireForJs(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (!args[0]->IsString()) {
    NOTREACHED() << "require() called with a non-string argument";
    return;
  }
  v8::Local<v8::String> module_name = args[0].As<v8::String>();
  args.GetReturnValue().Set(RequireForJsInner(module_name));
}
