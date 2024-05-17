static void methodQueryListListenerMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("methodQueryListListener", "TestObject", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_VOID(RefPtrWillBeRawPtr<MediaQueryListListener>, listener, MediaQueryListListener::create(ScriptValue(info[0], info.GetIsolate())));
    imp->methodQueryListListener(listener);
}
