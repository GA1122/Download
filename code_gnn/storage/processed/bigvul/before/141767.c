void setFunctionProperty(v8::Local<v8::Context> context, v8::Local<v8::Object> obj, const char* name, v8::FunctionCallback callback, v8::Local<v8::External> external)
{
    v8::Local<v8::String> funcName = toV8StringInternalized(context->GetIsolate(), name);
    v8::Local<v8::Function> func;
    if (!V8_FUNCTION_NEW_REMOVE_PROTOTYPE(context, callback, external, 0).ToLocal(&func))
        return;
    func->SetName(funcName);
    if (!obj->Set(context, funcName, func).FromMaybe(false))
        return;
}
