static v8::Handle<v8::Value> overloadedMethod1Callback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.overloadedMethod1");
    if (args.Length() == 1)
        return overloadedMethod11Callback(args);
    if ((args.Length() == 1 && (args[0]->IsNull() || args[0]->IsUndefined() || args[0]->IsString() || args[0]->IsObject())))
        return overloadedMethod12Callback(args);
    V8Proxy::throwTypeError();
    return notHandledByInterceptor();
}
