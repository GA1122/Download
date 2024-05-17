void V8Window::openerAttributeSetterCustom(v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    LocalDOMWindow* impl = toLocalDOMWindow(V8Window::toImpl(info.Holder()));
    ExceptionState exceptionState(ExceptionState::SetterContext, "opener", "Window", info.Holder(), info.GetIsolate());
    if (!BindingSecurity::shouldAllowAccessToFrame(info.GetIsolate(), impl->frame(), exceptionState)) {
        exceptionState.throwIfNeeded();
        return;
    }

    if (value->IsNull()) {
        ASSERT(impl->frame());
        impl->frame()->loader().setOpener(0);
    }

    info.Holder()->Delete(v8AtomicString(info.GetIsolate(), "opener"));

    if (info.This()->IsObject())
        v8::Handle<v8::Object>::Cast(info.This())->Set(v8AtomicString(info.GetIsolate(), "opener"), value);
}
