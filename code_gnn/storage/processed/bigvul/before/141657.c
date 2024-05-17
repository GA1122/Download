void createBoundFunctionProperty(v8::Local<v8::Context> context, v8::Local<v8::Object> console, const char* name, v8::FunctionCallback callback, const char* description = nullptr)
{
    v8::Local<v8::String> funcName = toV8StringInternalized(context->GetIsolate(), name);
    v8::Local<v8::Function> func;
    if (!V8_FUNCTION_NEW_REMOVE_PROTOTYPE(context, callback, console, 0).ToLocal(&func))
        return;
    func->SetName(funcName);
    if (description) {
        v8::Local<v8::String> returnValue = toV8String(context->GetIsolate(), description);
        v8::Local<v8::Function> toStringFunction;
        if (V8_FUNCTION_NEW_REMOVE_PROTOTYPE(context, returnDataCallback, returnValue, 0).ToLocal(&toStringFunction))
            func->Set(toV8StringInternalized(context->GetIsolate(), "toString"), toStringFunction);
    }
    if (!console->Set(context, funcName, func).FromMaybe(false))
        return;
}
