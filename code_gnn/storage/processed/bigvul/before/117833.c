static v8::Handle<v8::Value> methodWithOptionalArgCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.methodWithOptionalArg");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    if (args.Length() <= 0) {
        imp->methodWithOptionalArg();
        return v8::Handle<v8::Value>();
    }
    EXCEPTION_BLOCK(int, opt, toInt32(MAYBE_MISSING_PARAMETER(args, 0, DefaultIsUndefined)));
    imp->methodWithOptionalArg(opt);
    return v8::Handle<v8::Value>();
}
