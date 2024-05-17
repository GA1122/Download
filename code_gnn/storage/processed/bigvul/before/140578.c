v8::Local<v8::Object> V8DOMWrapper::createWrapper(v8::Isolate* isolate, v8::Local<v8::Object> creationContext, const WrapperTypeInfo* type, ScriptWrappable* scriptWrappable)
{
    ASSERT(!type->equals(&V8Window::wrapperTypeInfo));
    bool withSecurityCheck = !type->equals(&V8Location::wrapperTypeInfo);
    V8WrapperInstantiationScope scope(creationContext, isolate, withSecurityCheck);

    V8PerContextData* perContextData = V8PerContextData::from(scope.context());
    v8::Local<v8::Object> wrapper;
    if (perContextData) {
        wrapper = perContextData->createWrapperFromCache(type);
    } else {
        v8::Local<v8::Function> function;
        if (!type->domTemplate(isolate)->GetFunction(isolate->GetCurrentContext()).ToLocal(&function))
            return v8::Local<v8::Object>();
        if (!V8ObjectConstructor::newInstance(isolate, function).ToLocal(&wrapper))
            return v8::Local<v8::Object>();
    }

    if (type == &V8HTMLDocument::wrapperTypeInfo && !wrapper.IsEmpty())
        wrapper = wrapInShadowTemplate(wrapper, scriptWrappable, isolate);

    return wrapper;
}
