void V8TestObject::InstallFeatureName(
    ScriptState* script_state, v8::Local<v8::Object> instance) {
  V8PerContextData* per_context_data = script_state->PerContextData();
  v8::Local<v8::Object> prototype = per_context_data->PrototypeForType(
      V8TestObject::GetWrapperTypeInfo());
  v8::Local<v8::Function> interface = per_context_data->ConstructorForType(
      V8TestObject::GetWrapperTypeInfo());
  ALLOW_UNUSED_LOCAL(interface);
  InstallFeatureName(script_state->GetIsolate(), script_state->World(), instance, prototype, interface);
}
