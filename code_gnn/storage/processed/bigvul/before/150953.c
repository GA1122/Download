void importFunction(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  RELEASE_ASSERT(isolate && (args.Length() >= 1));
  String resourceFileName = toCoreString(
      args[0]->ToString(isolate->GetCurrentContext()).ToLocalChecked());
  String resourceData =
      loadResourceAsASCIIString(resourceFileName.utf8().data());
  RELEASE_ASSERT(resourceData.length());
  bool compileAndRunScript = true;
  if (args.Length() == 2) {
    RELEASE_ASSERT(args[1]->IsBoolean());
    compileAndRunScript = args[1].As<v8::Boolean>()->Value();
  }

  if (resourceFileName.endsWith(".js") && compileAndRunScript)
    compileAndRunPrivateScript(
        ScriptState::current(isolate), resourceFileName.replace(".js", ""),
        resourceData.utf8().data(), resourceData.length());
  args.GetReturnValue().Set(v8String(isolate, resourceData));
}
