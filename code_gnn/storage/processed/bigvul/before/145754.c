void ModuleSystem::SetLazyField(v8::Local<v8::Object> object,
                                const std::string& field,
                                const std::string& module_name,
                                const std::string& module_field,
                                v8::AccessorNameGetterCallback getter) {
  CHECK(field.size() < v8::String::kMaxLength);
  CHECK(module_name.size() < v8::String::kMaxLength);
  CHECK(module_field.size() < v8::String::kMaxLength);
  v8::HandleScope handle_scope(GetIsolate());
  v8::Local<v8::Object> parameters = v8::Object::New(GetIsolate());
  v8::Local<v8::Context> context = context_->v8_context();
  SetProperty(context, parameters, kModuleName,
              ToV8StringUnsafe(GetIsolate(), module_name.c_str()));
  SetProperty(context, parameters, kModuleField,
              ToV8StringUnsafe(GetIsolate(), module_field.c_str()));
  auto maybe = object->SetAccessor(
      context, ToV8StringUnsafe(GetIsolate(), field.c_str()), getter, NULL,
      parameters);
  CHECK(IsTrue(maybe));
}
