static v8::Handle<v8::Value> methodWithOptionalStringCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.methodWithOptionalString");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    if (args.Length() <= 0) {
        imp->methodWithOptionalString();
        return v8::Handle<v8::Value>();
    }
    STRING_TO_V8PARAMETER_EXCEPTION_BLOCK(V8Parameter<>, str, MAYBE_MISSING_PARAMETER(args, 0, DefaultIsUndefined));
    imp->methodWithOptionalString(str);
    return v8::Handle<v8::Value>();
}
