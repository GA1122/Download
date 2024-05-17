static void dispatchEventMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "dispatchEvent", "TestObjectPython", info.Holder(), info.GetIsolate());
    EventTarget* impl = V8TestObjectPython::toNative(info.Holder());
    if (DOMWindow* window = impl->toDOMWindow()) {
        if (!BindingSecurity::shouldAllowAccessToFrame(info.GetIsolate(), window->frame(), exceptionState)) {
            exceptionState.throwIfNeeded();
            return;
        }
        if (!window->document())
            return;
    }
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Event*, event, V8Event::toNativeWithTypeCheck(info.GetIsolate(), info[0]));
    bool result = imp->dispatchEvent(event, exceptionState);
    if (exceptionState.throwIfNeeded())
        return;
    v8SetReturnValueBool(info, result);
}
