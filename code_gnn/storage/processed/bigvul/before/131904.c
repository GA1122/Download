static void voidMethodEventTargetArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodEventTargetArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(EventTarget*, eventTargetArg, V8DOMWrapper::isDOMWrapper(info[0]) ? toWrapperTypeInfo(v8::Handle<v8::Object>::Cast(info[0]))->toEventTarget(v8::Handle<v8::Object>::Cast(info[0])) : 0);
    imp->voidMethodEventTargetArg(eventTargetArg);
}
