void ModuleSystem::RunString(const std::string& code, const std::string& name) {
  v8::HandleScope handle_scope(GetIsolate());
  v8::Local<v8::String> v8_code;
  v8::Local<v8::String> v8_name;
  if (!ToV8String(GetIsolate(), code.c_str(), &v8_code) ||
      !ToV8String(GetIsolate(), name.c_str(), &v8_name)) {
    Warn(GetIsolate(), "Too long code or name.");
    return;
  }
  RunString(v8_code, v8_name);
}
