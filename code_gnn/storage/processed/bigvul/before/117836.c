static v8::Handle<v8::Value> methodWithOptionalStringIsUndefinedCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.methodWithOptionalStringIsUndefined");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    STRING_TO_V8PARAMETER_EXCEPTION_BLOCK(V8Parameter<>, str, MAYBE_MISSING_PARAMETER(args, 0, DefaultIsUndefined));
    imp->methodWithOptionalStringIsUndefined(str);
    return v8::Handle<v8::Value>();
}
