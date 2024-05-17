static v8::Handle<v8::Value> classMethodWithOptionalCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.classMethodWithOptional");
    if (args.Length() <= 0) {
        return v8::Integer::New(TestObj::classMethodWithOptional());
    }
    EXCEPTION_BLOCK(int, arg, toInt32(MAYBE_MISSING_PARAMETER(args, 0, DefaultIsUndefined)));
    return v8::Integer::New(TestObj::classMethodWithOptional(arg));
}
