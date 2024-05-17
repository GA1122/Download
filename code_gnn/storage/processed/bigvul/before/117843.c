static v8::Handle<v8::Value> overloadedMethodCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.overloadedMethod");
    if ((args.Length() == 2 && (args[0]->IsNull() || V8TestObj::HasInstance(args[0])) && (args[1]->IsNull() || args[1]->IsUndefined() || args[1]->IsString() || args[1]->IsObject())))
        return overloadedMethod1Callback(args);
    if ((args.Length() == 1 && (args[0]->IsNull() || V8TestObj::HasInstance(args[0]))) || (args.Length() == 2 && (args[0]->IsNull() || V8TestObj::HasInstance(args[0]))))
        return overloadedMethod2Callback(args);
    if ((args.Length() == 1 && (args[0]->IsNull() || args[0]->IsUndefined() || args[0]->IsString() || args[0]->IsObject())))
        return overloadedMethod3Callback(args);
    if (args.Length() == 1)
        return overloadedMethod4Callback(args);
    if ((args.Length() == 1 && (args[0]->IsNull() || args[0]->IsFunction())))
        return overloadedMethod5Callback(args);
    if ((args.Length() == 1 && (args[0]->IsNull() || V8DOMStringList::HasInstance(args[0]))))
        return overloadedMethod6Callback(args);
    if ((args.Length() == 1 && (args[0]->IsNull() || args[0]->IsArray())))
        return overloadedMethod7Callback(args);
    V8Proxy::throwTypeError();
    return notHandledByInterceptor();
}
