void V8Window::frameElementAttributeGetterCustom(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    LocalDOMWindow* impl = toLocalDOMWindow(V8Window::toImpl(info.Holder()));
    ExceptionState exceptionState(ExceptionState::GetterContext, "frame", "Window", info.Holder(), info.GetIsolate());
    if (!BindingSecurity::shouldAllowAccessToNode(info.GetIsolate(), impl->frameElement(), exceptionState)) {
        v8SetReturnValueNull(info);
        exceptionState.throwIfNeeded();
        return;
    }

    v8::Handle<v8::Value> creationContext = toV8(&impl->frameElement()->document(), info.Holder(), info.GetIsolate());
    RELEASE_ASSERT(!creationContext.IsEmpty());
    v8::Handle<v8::Value> wrapper = toV8(impl->frameElement(), v8::Handle<v8::Object>::Cast(creationContext), info.GetIsolate());
    v8SetReturnValue(info, wrapper);
}
