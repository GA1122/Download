static v8::Handle<v8::Value> methodWithCallbackAndOptionalArgCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.methodWithCallbackAndOptionalArg");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    RefPtr<TestCallback> callback;
    if (args.Length() > 0 && !args[0]->IsNull() && !args[0]->IsUndefined()) {
        if (!args[0]->IsFunction())
            return throwError(TYPE_MISMATCH_ERR, args.GetIsolate());
        callback = V8TestCallback::create(args[0], getScriptExecutionContext());
    }
    imp->methodWithCallbackAndOptionalArg(callback);
    return v8::Handle<v8::Value>();
}
