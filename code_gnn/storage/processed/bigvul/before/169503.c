void ModuleSystem::LazyFieldGetterInner(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info,
    RequireFunction require_function) {
  CHECK(!info.Data().IsEmpty());
  CHECK(info.Data()->IsObject());
  v8::Isolate* isolate = info.GetIsolate();
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::Object> parameters = v8::Local<v8::Object>::Cast(info.Data());
  v8::Local<v8::Context> context = parameters->CreationContext();
  v8::Local<v8::Object> global(context->Global());
  v8::Local<v8::Value> module_system_value;
  if (!GetPrivate(context, global, kModuleSystem, &module_system_value) ||
      !module_system_value->IsExternal()) {
    Warn(isolate,
         "Module system has been deleted, does extension view exist?");
    return;
  }

  ModuleSystem* module_system = static_cast<ModuleSystem*>(
      v8::Local<v8::External>::Cast(module_system_value)->Value());

  v8::Local<v8::Value> v8_module_name;
  if (!GetProperty(context, parameters, kModuleName, &v8_module_name)) {
    Warn(isolate, "Cannot find module.");
    return;
  }
  std::string name = *v8::String::Utf8Value(v8_module_name);

  v8::Context::Scope context_scope(context);
  NativesEnabledScope natives_enabled_scope(module_system);

  v8::TryCatch try_catch(isolate);
  v8::Local<v8::Value> module_value;
  if (!(module_system->*require_function)(name).ToLocal(&module_value)) {
    module_system->HandleException(try_catch);
    return;
  }

  v8::Local<v8::Object> module = v8::Local<v8::Object>::Cast(module_value);
  v8::Local<v8::Value> field_value;
  if (!GetProperty(context, parameters, kModuleField, &field_value)) {
    module_system->HandleException(try_catch);
    return;
  }
  v8::Local<v8::String> field;
  if (!field_value->ToString(context).ToLocal(&field)) {
    module_system->HandleException(try_catch);
    return;
  }

  if (!IsTrue(module->Has(context, field))) {
    std::string field_str = *v8::String::Utf8Value(field);
    Fatal(module_system->context_,
          "Lazy require of " + name + "." + field_str + " did not set the " +
              field_str + " field");
    return;
  }

  v8::Local<v8::Value> new_field;
  if (!GetProperty(context, module, field, &new_field)) {
    module_system->HandleException(try_catch);
    return;
  }

  CHECK(!new_field.IsEmpty());

  v8::Local<v8::Value> val = info.This();
  if (val->IsObject()) {
    v8::Local<v8::Object> object = v8::Local<v8::Object>::Cast(val);
    object->Delete(context, property);
    SetProperty(context, object, property, new_field);
  } else {
    NOTREACHED();
  }
  info.GetReturnValue().Set(new_field);
}
